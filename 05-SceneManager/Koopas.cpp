#include "Koopas.h"
#include "Block.h"

CKoopas::CKoopas(int tag)
{
	this->start_x = x;
	this->start_y = y;
	this->start_tag = tag;
	if (tag == KOOPAS_GREEN) {
		this->nx = -1;
	}
	this->nx = -1;
	this->SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	bottom = y + KOOPAS_BBOX_HEIGHT;
}


void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}

	else
	{
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			if (!dynamic_cast<CBlock*>(e->obj)) {
				vx = -vx;
				nx = -nx;
			}
			else {
				CBlock* block = dynamic_cast<CBlock*>(e->obj);
				block->SetIsBlocking(1);
			}
		}
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopas::Render()
{
	int ani = -1;
	if (state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_DEATH)
		ani = KOOPAS_ANI_SHELL_UP;
	else if (state == KOOPAS_STATE_IN_SHELL)
		ani = KOOPAS_ANI_SHELL;
	else if (state == KOOPAS_STATE_SPINNING)
	{
		if (vx < 0)
			ani = KOOPAS_ANI_SPIN_LEFT;
		else
			ani = KOOPAS_ANI_SPIN_RIGHT;
	}
	else
	{
		if (this->nx < 0)
			ani = KOOPAS_ANI_WALKING_LEFT;
		else
			ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		vx = this->nx * KOOPAS_WALKING_SPEED;
	/*	vy = KOOPAS_WALKING_SPEED;*/
		break;
	case KOOPAS_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_DEATH:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_SPINNING:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_IN_SHELL:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_SHELL_UP:
		vy = -KOOPAS_SHELL_DEFLECT_SPEED;
		break;
	}
}
