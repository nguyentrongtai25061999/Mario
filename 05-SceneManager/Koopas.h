#pragma once
#include "GameObject.h"

#define KOOPAS_GRAVITY					0.002f
#define KOOPAS_WALKING_SPEED			0.03f
#define KOOPAS_SHELL_DEFLECT_SPEED		0.4f


#define KOOPAS_BBOX_WIDTH		16
#define KOOPAS_BBOX_HEIGHT		20



#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_SHELL_UP	200
#define KOOPAS_STATE_SPINNING	300
#define KOOPAS_STATE_IN_SHELL	400
#define KOOPAS_STATE_DEATH		500
#define KOOPAS_STATE_INACTIVE	600

#define KOOPAS_ANI_WALKING_RIGHT	0
#define KOOPAS_ANI_SPIN_RIGHT		1
#define KOOPAS_ANI_WALKING_LEFT		2
#define KOOPAS_ANI_SPIN_LEFT		3
#define KOOPAS_ANI_SHELL			4
#define KOOPAS_ANI_SHAKE			5
#define KOOPAS_ANI_SHELL_UP			6
#define KOOPAS_BBOX_SHELL_HEIGHT	14


#define KOOPAS_GREEN		0
#define KOOPAS_RED			1
#define KOOPAS_TURN_DIFF	4

class CKoopas : public CGameObject
{
	ULONGLONG dt = 0;
public:
	float start_x = 0, start_y = 0;
	int start_tag = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	CKoopas(int tag);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);
	bool CalTurnable(LPGAMEOBJECT object);
};