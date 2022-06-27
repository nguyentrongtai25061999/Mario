#pragma once
#include "GameObject.h"

#define PIECE_BBOX_WIDTH  8
#define PIECE_BBOX_HEIGHT 8
#define PIECE_ANI_SET_ID	126
class BreakPiece :
	public CGameObject
{
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	BreakPiece(float nx, float ny) { this->vx = nx; this->vy = ny; };
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; }
};

