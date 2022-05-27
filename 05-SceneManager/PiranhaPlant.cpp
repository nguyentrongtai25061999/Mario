#include "PiranhaPlant.h"
#include "Game.h"
#include "PlayScene.h"

void PiranhaPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
}

PiranhaPlant::PiranhaPlant()
{
	SetState(PIRANHAPLANT_STATE_DARTING);
}

void PiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (y <= limitY && vy < 0)
	{
		y = limitY;
		SetState(PIRANHAPLANT_STATE_BITING);
	}
	CGameObject::Update(dt, coObjects);

	y += vy * dt;
	x += vx * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		float mLeft, mTop, mRight, mBottom;
		float mWidth = mario->GetWidth();
			mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			DebugOut(L"Mario !=null \n");
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom)) {
				if (mario->GetLevel() != MARIO_LEVEL_SMALL)
				{
					mario->SetLevel(mario->GetLevel()-1);
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					mario->SetState(MARIO_STATE_DIE);
				}
			}
	}
}

void PiranhaPlant::Render()
{
	int ani = PIRANHAPLANT_ANI_DARTING;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void PiranhaPlant::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
		case PIRANHAPLANT_STATE_DARTING:
			vy = -PIRANHAPLANT_DARTING_SPEED;
			break;
		case PIRANHAPLANT_STATE_BITING:
			vy = 0;
			StartBitting();
			break;
	}
}