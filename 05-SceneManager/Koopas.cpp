#include "Koopas.h"
#include "Block.h"
#include "PlayScene.h"
#include "Brick.h"
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
				block->SetIsBlocking(0);
			}
		}
	}
	if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
	if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
}
void CKoopas::OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	GetBoundingBox(mLeft, mTop, mRight, mBottom);
	e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);

	if (e->ny < 0) {
		this->vy = 0;
		if (state == KOOPAS_STATE_SHELL_UP)
			vx = 0;
		if (tag == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
		{
			if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
				if (CalTurnable(e->obj))
				{
					this->nx = -1;
					vx = this->nx * KOOPAS_WALKING_SPEED;
				}
			if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
				if (CalTurnable(e->obj))
				{
					this->nx = 1;
					vx = this->nx * KOOPAS_WALKING_SPEED;
				}
		}
		if (KOOPAS_GREEN) {
			this->nx = -1;
			vx = this->nx * KOOPAS_WALKING_SPEED;
		}
	}
	if (e->nx != 0)
	{
		if (ceil(mBottom) != oTop)
		{
			//vx = -vx;
			this->nx = -this->nx;
		}
	}
}
void CKoopas::OnCollisionWithBlock(LPCOLLISIONEVENT e) {
	if (e->ny < 0)
	{
		vy = 0.002f;
		if (state == KOOPAS_STATE_SHELL_UP)
			vx = 0;
		if (tag == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
		{
			if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
				if (CalTurnable(e->obj))
				{
					this->nx = -1;
					vx = this->nx * KOOPAS_WALKING_SPEED;
				}
			if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
				if (CalTurnable(e->obj))
				{
					this->nx = 1;
					vx = this->nx * KOOPAS_WALKING_SPEED;
				}
		}
	}
	else
	{
		if (e->nx != 0)
			x += vx * this->dt;
		if (state == KOOPAS_STATE_SHELL_UP && e->ny > 0)
			y += vy * this->dt;
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
bool CKoopas::CalTurnable(LPGAMEOBJECT object)
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> coObjects = currentScene->GetObjects();
	for (UINT i = 0; i < coObjects.size(); i++)
		if (dynamic_cast<CBlock*>(coObjects[i])|| dynamic_cast<CBrick*>(coObjects[i]))
			if (abs(coObjects[i]->y == object->y))
			{
				if (nx > 0)
					if (coObjects[i]->x > object->x && coObjects[i]->x - 16 < object->x + 16)
						return false;
				if (nx < 0)
					if (coObjects[i]->x + 16 > object->x - 16 && coObjects[i]->x < object->x)
						return false;
			}
	return true;
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		vx = this->nx * KOOPAS_WALKING_SPEED;
		vy = KOOPAS_WALKING_SPEED;
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
