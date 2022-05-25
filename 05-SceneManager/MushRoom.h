#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_SPEED 0.07f
#define MUSHROOM_STATE_IDLE	0
#define MUSHROOM_GRAVITY 0.2f

class CMushRoom :public CGameObject
{
	bool isAppear;
	int typeMushRoom;
public:
	virtual void Render();

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + MUSHROOM_BBOX_WIDTH;
		b = y + MUSHROOM_BBOX_HEIGHT;
	};

	void SetState(int state);
	void SetAppear(bool appear) {
		isAppear = appear;
	}
	bool GetAppear() { return isAppear; }
	int GetTypeMushRoom() { return typeMushRoom; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	CMushRoom(int type = 0);
	~CMushRoom();
};