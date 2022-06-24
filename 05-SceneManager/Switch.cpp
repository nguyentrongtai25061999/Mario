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
	CGameObject::Update(dt);
}

void Switch::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	}
}

void Switch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SWITCH_BBOX_WIDTH;
	b = y + SWITCH_BBOX_HEIGHT;
}
