#include "Leaf.h"
#include "Mario.h"
#include "PlayScene.h"

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LEAF_BBOX_WIDTH;
	b = y + LEAF_BBOX_HEIGHT;
}

void CLeaf::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}

void CLeaf::Render()
{
	int ani = LEAF_ANI_SET_ID;
	
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	x += vx * dt;
	y += vy * dt;
	CGameObject::Update(dt);

	}

void CLeaf::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_IDLE:
		vx = vy = 0;
		break;
	}
}