#include "Goomba.h"
#include "debug.h"
#include "Brick.h"
#include "PlayScene.h"
#include "Mario.h"
CGoomba::CGoomba(int tag)
{
	this->ax = 0;//this o class nao la cai do
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	nx = -1;
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE_BY_MARIO)
	{
		left = top = right = bottom = 0;
		return;
	}
	left = x;
	top = y;
	right = x + GOOMBA_NORMAL_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
	if (tag == GOOMBA_RED)
	{
		right = x + GOOMBA_RED_BBOX_WIDTH;
		bottom = y + GOOMBA_RED_BBOX_WINGS_HEIGHT;
		if (state == GOOMBA_STATE_RED_WINGSWALKING)
			bottom = y + GOOMBA_RED_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (dynamic_cast<CBrick*>(e->obj)) {
		if (e->ny != 0)
		{
			vy = 0;
			if (e->ny < 0 && tag == GOOMBA_RED && state != GOOMBA_STATE_DIE)
			{
				if (!walkingTimer)
				{
					//DebugOut(L"OnCollisionWith");
					if (jumpingStacks == GOOMBA_RED_JUMPING_STACKS)
					{
						SetState(GOOMBA_STATE_RED_HIGHJUMPING);
						jumpingStacks = -1;
						y -= GOOMBA_RED_BBOX_WINGS_HEIGHT - GOOMBA_RED_BBOX_HEIGHT + 5; // 5 is a option can edit
						//DebugOut(L"OnCollisionWith if 1========\n");
					}
					else
					{
						if (jumpingStacks == -1)
						{
							//DebugOut(L"OnCollisionWith if 2========\n");
							SetState(GOOMBA_STATE_RED_WINGSWALKING);
						}
						else
						{
							SetState(GOOMBA_STATE_RED_JUMPING);
							//DebugOut(L"OnCollisionWith else 2========\n");
						}

						jumpingStacks++;
						//DebugOut(L"OnCollisionWith else 1========\n");
					}
				}
				else
					ay = GOOMBA_GRAVITY;
			}
			else if (e->ny > 0)
				ay = GOOMBA_GRAVITY;
		}
	}
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if ((tag == GOOMBA_RED) && state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_BY_MARIO)
	{
		if (GetTickCount64() - chasingTimer >= GOOMBA_RED_TIME_CHASING && chasingTimer)
		{
			DebugOut(L"chasingtimer");
			chasingTimer = 0;
		}
	}
	/* limit y*/
	if (vy < -GOOMBA_JUMP_SPEED && state == GOOMBA_STATE_RED_JUMPING)
	{
		vy = -GOOMBA_JUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}
	if (vy < -GOOMBA_HIGHJUMP_SPEED && state == GOOMBA_STATE_RED_HIGHJUMPING)
	{
		vy = -GOOMBA_HIGHJUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int ani = 0;
	switch (tag)
	{
	case GOOMBA_NORMAL:
		ani = GOOMBA_NORMAL_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE)
			ani = GOOMBA_NORMAL_ANI_DIE;
		break;
	case GOOMBA_RED:
		ani = GOOMBA_RED_ANI_WINGSWALKING;
		if (state == GOOMBA_STATE_RED_JUMPING || state == GOOMBA_STATE_RED_HIGHJUMPING)
			ani = GOOMBA_RED_ANI_JUMPING;
		if (state == GOOMBA_STATE_DIE_BY_MARIO)
			ani = GOOMBA_RED_ANI_WALKING;
		break;
	case GOOMBA_RED_NORMAL:
		ani = GOOMBA_RED_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE)
			ani = GOOMBA_RED_ANI_DIE;
		break;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += GOOMBA_NORMAL_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_DIE_BY_MARIO:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = -vx;
		ay = GOOMBA_GRAVITY;
		die_start = GetTickCount64();
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_RED_WINGSWALKING:
		ay = GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_RED_JUMPING:
		ay = -GOOMBA_GRAVITY;
		break;
	case GOOMBA_STATE_RED_HIGHJUMPING:
		ay = -GOOMBA_GRAVITY;
		break;
	}
}
