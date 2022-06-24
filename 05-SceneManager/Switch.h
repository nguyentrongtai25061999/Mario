#pragma once
#include "GameObject.h"

#define SWITCH_BBOX_WIDTH			16
#define SWITCH_BBOX_HEIGHT			16
#define SWITCH_ANI_SET_ID		77
#define SWITCH_ANI_IDLE		0
#define SWITCH_STATE_IDLE		0

class Switch :public CGameObject
{
	bool isAppear = false;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetState(int state);
	void SetAppear(bool appear) { this->isAppear = appear; }
	bool GetIsAppear() { return isAppear; }
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
};

