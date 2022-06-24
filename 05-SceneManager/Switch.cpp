#include "Switch.h"
#include "Coin.h"
#include "BreakableBrick.h"
#include "PlayScene.h"

void Switch::Render() {
	if (!isAppear || isDeleted)
		return;
		animation_set->at(SWITCH_ANI_IDLE)->Render(x, y);
	//RenderBoundingBox();
}

void Switch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted)
		return;
	CGameObject::Update(dt);
	if (state == SWITCH_STATE_UP)
	{
		y += vy * dt;
		if (start_y - y >= SWITCH_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - SWITCH_BBOX_HEIGHT - 0.1f;
			SetState(SWITCH_STATE_IDLE);
		}
	}
}

void Switch::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case SWITCH_STATE_UP:
		vy = -0.05f;
		start_y = y;
		break;
	}
}

void Switch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SWITCH_BBOX_WIDTH;
	b = y + SWITCH_BBOX_HEIGHT;
}
