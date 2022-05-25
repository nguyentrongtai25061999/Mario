#include "MushRoom.h"
#include "PlayScene.h"
CMushRoom::CMushRoom(int type) {
	SetAppear(false);
	typeMushRoom = type;
}

void CMushRoom::Render() {
	if (isAppear && !isDeleted) {
	 animation_set->at(0)->Render(x, y);
	}
	RenderBoundingBox();
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isDeleted) return;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMushRoom::OnNoCollision(DWORD dt) {
	
		x += vx * dt;
		y += vy * dt;
}


void CMushRoom::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CMushRoom::SetState(int state) {
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vy = vx = 0;
		break;
	}
}