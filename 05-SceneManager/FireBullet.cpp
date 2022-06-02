#include "FireBullet.h"
#include "PiranhaPlantFire.h"
#include "Mario.h"
#include "PlayScene.h"
FireBullet::FireBullet(float bx, float by, bool up, bool right) {
	if (up) {
		y = by;
		vy = -BULLET_SPEED_Y;
	}
	else {
		y = by + BULLET_BBOX_HEIGHT;
		vy = BULLET_SPEED_Y;
	}

	if (right)
	{
		x = bx + PIRANHAPLANT_BBOX_WIDTH;
		vx = BULLET_SPEED_X;
	}
	else
	{
		x = bx - BULLET_BBOX_WIDTH;
		vx = -BULLET_SPEED_X;
	}
}

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	x += vx * dt;
	y += vy * dt;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		float mLeft, mTop, mRight, mBottom;
		float mWidth = mario->GetWidth();
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom)) {
			Delete();
			if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			{
				mario->SetLevel(mario->GetLevel() - 1);
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				mario->SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void FireBullet::Render()
{
	int ani = 0;
	if (vx > 0)
		ani = BULLET_ANI_RIGHT;
	else
		ani = BULLET_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void FireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BULLET_BBOX_WIDTH;
	b = y + BULLET_BBOX_HEIGHT;
}