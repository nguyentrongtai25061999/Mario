#include "PiranhaPlantFire.h"
#include "PlayScene.h"

PiranhaPlantFire::PiranhaPlantFire(int tag) {
	this->tag = tag;
	SetState(PIRANHAPLANT_STATE_DARTING);
}

void PiranhaPlantFire::Render()
{
	int ani = PIRANHAPLANT_STATE_DARTING;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void PiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
	y += vy * dt;

}


void PiranhaPlantFire::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y -PIRANHAPLANT_BBOX_HEIGHT;
}

void PiranhaPlantFire::SetState(int state) {
	CGameObject::Update(state);
	switch (state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		break;
	}
}
