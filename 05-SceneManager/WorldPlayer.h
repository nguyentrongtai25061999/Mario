#pragma once
#include "GameObject.h"
#include "Game.h"
#include "FireBullet.h"
#include "Utils.h"
#include "Portal.h"


#define PLAYER_SPEED	0.1f

#define PLAYER_STATE_IDLE			0
#define PLAYER_STATE_RIGHT			100
#define PLAYER_STATE_LEFT			111
#define PLAYER_STATE_UP				200
#define PLAYER_STATE_DOWN			201
#define PLAYER_BBOX_WIDTH			8	
class CWorldPlayer : public CGameObject
{
	float start_x;// initial position of Mario at scene
	float start_y;
	int level;
public:
	bool cgLeft, cgRight, cgUp, cgDown;
	int sceneId;
	CWorldPlayer(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int vState);
	void SetLevel(int l) { level = l; };
	void SetMove(bool cLeft, bool cUp, bool cRight, bool cDown)
	{
		cgLeft = cLeft;
		cgRight = cRight;
		cgUp = cUp;
		cgDown = cDown;
	};
	void ChooseScene()
	{
		CGame::GetInstance()->InitiateSwitchScene(1);
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithPortalOrStop(LPCOLLISIONEVENT e);
};
