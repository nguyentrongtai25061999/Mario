#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 16
#define LEAF_GRAVITY		0.025f
#define LEAF_STATE_IDLE		0
#define LEAF_ANI_SET_ID		36

class CLeaf :
	public CGameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual int IsCollidable() { return 0; };
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
};
