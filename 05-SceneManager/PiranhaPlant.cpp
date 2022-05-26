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