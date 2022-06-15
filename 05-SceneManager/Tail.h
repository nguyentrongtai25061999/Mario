#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH		8
#define TAIL_BBOX_HEIGHT	6
#define TAIL_HIT_SPRITE_ID	10998	
#define TAIL_HIT_TIME 25

class CTail :public CGameObject
{
	ULONGLONG hit_start = 0;
	
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	CTail(float x, float y) { this->x = x; this->y = y; }
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void ShowHitEffect() { hit_start = GetTickCount64(); };
};
