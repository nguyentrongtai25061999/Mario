#pragma once
#include "GameObject.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.03f


#define KOOPAS_BBOX_WIDTH		16
#define KOOPAS_BBOX_HEIGHT		26


#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DEATH	500

#define KOOPAS_ANI_WALKING_RIGHT	0
#define KOOPAS_GREEN		0
#define KOOPAS_RED			1

class CKoopas : public CGameObject
{
	float start_x = 0, start_y = 0;
	int start_tag = 0;

public:
	CKoopas(float x, float y);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CKoopas(int tag);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};