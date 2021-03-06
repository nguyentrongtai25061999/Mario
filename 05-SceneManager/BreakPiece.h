#pragma once
#include "GameObject.h"

#define PIECE_BBOX_WIDTH  8
#define PIECE_BBOX_HEIGHT 8
#define PIECE_ANI_SET_ID	126
#define PIECE_SPEED 0.075f
#define PIECE_TIME 500
class BreakPiece :public CGameObject
{
	ULONGLONG start_exist = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

public:
	BreakPiece(float nx, float ny) { this->vx = nx * PIECE_SPEED; this->vy = ny * PIECE_SPEED; StartExisted(); };
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; }
	void StartExisted() { start_exist = GetTickCount64(); }
};

