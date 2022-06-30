#include <algorithm>
#include "Utils.h"
#include "WorldPlayer.h"
#include "WorldScene.h"
#include "debug.h"
#include "WorldMapObjects.h"

CWorldPlayer::CWorldPlayer(float x, float y) : CGameObject()
{
	SetLevel(2);
	SetState(PLAYER_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	SetMove(false, false, true, false);
}

void CWorldPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 2; top = y + 2; right = left + PLAYER_BBOX_WIDTH - 4; bottom = top + PLAYER_BBOX_WIDTH - 4;
}

void CWorldPlayer::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};
void CWorldPlayer::OnCollisionWith(LPCOLLISIONEVENT e)
{
	SetState(PLAYER_STATE_IDLE);
	DebugOut(L"OnCollisionWith");
	if (e->obj->tag == OBJECT_TYPE_PORTAL || e->obj->tag == OBJECT_TYPE_STOP)
	{
		OnCollisionWithPortalOrStop(e);
	}
}
void CWorldPlayer::OnCollisionWithPortalOrStop(LPCOLLISIONEVENT e)
{
	CWorldMapObject* tmp = dynamic_cast<CWorldMapObject*>(e->obj);
	bool cl, cr, cu, cd;
	tmp->GetMove(cl, cu, cr, cd);
	SetMove(cl, cu, cr, cd);

	if (e->obj->tag == OBJECT_TYPE_PORTAL)
		sceneId = tmp->GetSceneId();
	else
		sceneId = -1;
}
void CWorldPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CWorldPlayer::Render()
{
	animation_set->at(level - 1)->Render(x, y);
	RenderBoundingBox();
}
void CWorldPlayer::SetState(int state)
{
	SetMove(false, false, false, false);
	switch (state)
	{
	case PLAYER_STATE_IDLE:
		vx = vy = 0;
		break;
	case PLAYER_STATE_RIGHT:
		vx = PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_LEFT:
		vx = -PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_UP:
		vx = 0;
		vy = -PLAYER_SPEED;
		break;
	case PLAYER_STATE_DOWN:
		vx = 0;
		vy = PLAYER_SPEED;
		break;
	}
	CGameObject::SetState(state);
}

