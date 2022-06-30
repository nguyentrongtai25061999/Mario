#include "PiranhaPlantFire.h"
#include "PlayScene.h"
#include "FireBullet.h"
#include "Mario.h"
#include "PiranhaPlant.h"
PiranhaPlantFire::PiranhaPlantFire(int tag) {
	this->tag = tag;
	SetState(PIRANHAPLANT_STATE_INACTIVE);
}

void PiranhaPlantFire::Render()
{
	int ani = PIRANHAPLANT_ANI_DEATH;
	if (state != PIRANHAPLANT_STATE_DEATH && dying_start == 0)
	{
		if (Up)
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_UP;
			else
				ani = PIRANHAPLANT_ANI_LEFT_UP;
		else
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_DOWN;
			else
				ani = PIRANHAPLANT_ANI_LEFT_DOWN;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void PiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (GetTickCount64() - dying_start >= PIRANHAPLANT_DIYING_TIME && dying_start != 0)
		isDeleted = true;

	if (state == PIRANHAPLANT_STATE_DEATH)
		return;

	CGameObject::Update(dt);
	y += vy * dt;
	GetDirect();
	if (y <= limitY && vy < 0)
	{
		y = limitY;
		vy = 0;
		StartAim();
		//SetState(PIRANHAPLANT_STATE_INACTIVE);
	}
	if (y >= limitY + BBHeight && vy > 0)
	{
		y = limitY + BBHeight + 12;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
		StartDelay();
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (GetTickCount64() - aim_start >= PIRANHAPLANT_AIM_TIME && aim_start != 0)
	{
		aim_start = 0;
		SetState(PIRANHAPLANT_STATE_SHOOTING);
		StartDelay();
	}
	if (GetTickCount64() - delay_start >= PIRANHAPLANT_DELAY_TIME && delay_start != 0)
	{
		delay_start = 0;
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
	}
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		float mLeft, mTop, mRight, mBottom;
		float mWidth = mario->GetWidth();
		if (x - 60 <= floor(mario->x)
			&& floor(mario->x) + (float)mWidth <= x
			&& state == PIRANHAPLANT_STATE_INACTIVE && delay_start == 0
			&& !isMarioInActiveZone
			|| floor(mario->x) > x && floor(mario->x) < x + PIRANHAPLANT_BBOX_WIDTH + 60
			&& state == PIRANHAPLANT_STATE_INACTIVE && delay_start == 0
			&& !isMarioInActiveZone)
		{
			SetState(PIRANHAPLANT_STATE_DARTING);
			isMarioInActiveZone = true;
		}
		if (floor(mario->x) < x - 60 || floor(mario->x) > x + PIRANHAPLANT_BBOX_WIDTH + 60) {
			isMarioInActiveZone = false;
		}
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom)) {
			mario->HandleBasicMarioDie();
		}
		if (mario->GetLevel() == MARIO_LEVEL_TAIL) {
			mario->tail->GetBoundingBox(mLeft, mTop, mRight, mBottom);

			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->isTuring) {
				mario->AddScore(x, y, 100);
				SetState(PIRANHAPLANT_STATE_DEATH);
				 DebugOut(L">>> PRANHAPLANT DIE >>> \n");
				mario->tail->ShowHitEffect();
			}
		}
	}
}


void PiranhaPlantFire::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + BBHeight;
}

void PiranhaPlantFire::SetState(int state) {
	CGameObject::Update(state);
	switch (state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		break;
	case PIRANHAPLANT_STATE_SHOOTING:
		vy = 0;
		Shoot();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		StartDying();
		break;
	}
}
void PiranhaPlantFire::Shoot() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	this->bullet = new FireBullet(x, y, Up, Right);

	//! Basic setup for bullet object
	int ani_set_id = BULLET_ANI_SET_ID;
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	bullet->SetAnimationSet(ani_set);
	currentScene->AddObject(bullet);
}
void PiranhaPlantFire::GetDirect() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int mHeight;
	if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		mHeight = MARIO_SMALL_BBOX_HEIGHT;
	else
		mHeight = MARIO_BIG_BBOX_HEIGHT;

	if (mario->y + mHeight < limitY + BBHeight)
		Up = true;
	else
		Up = false;
	if (mario->x <= x)
		Right = false;
	else
		Right = true;
};