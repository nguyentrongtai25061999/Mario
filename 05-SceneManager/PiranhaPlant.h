#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "debug.h"

#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_BBOX_HEIGHT				24
#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_ANI_DARTING				0
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_STATE_BITING				100


class PiranhaPlant :
	public CGameObject
{
	ULONGLONG biting_start = 0;
	float limitY = 0;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void SetLimitY(float ly) { limitY = ly - PIRANHAPLANT_BBOX_HEIGHT; DebugOut(L"LimitY::%f\n", limitY); }
	void StartBitting() { biting_start = GetTickCount64(); }
	PiranhaPlant();
};
