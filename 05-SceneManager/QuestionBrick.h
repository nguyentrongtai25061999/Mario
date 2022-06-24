#pragma once
#include "GameObject.h"

#define QUESTION_BRICK_NORMAL	0
#define QUESTION_BRICK_ANI_NORMAL	0
#define QUESTION_BRICK_HIT	1
#define QUESTION_BRICK_ANI_HIT	1
#define QUESTIONBRICK_SPEED		0.05f
#define ITEM_COIN_QUESTION_BRICK_COIN	0
#define COIN_ANI_SET_ID 6
#define ITEM_CUSTOM		1
#define ITEM_MUSHROOM_RED 3
#define ITEM_LEAF	2
#define ITEM_MUSHROOM_ANI_SET_ID 37
#define ITEM_SWITCH		6


class QuestionBrick :public CGameObject
{
	LPGAMEOBJECT item;
	int tag;
	int totalItems = 1;
public:
	CGameObject* obj = NULL;
	QuestionBrick(int tag = ITEM_COIN_QUESTION_BRICK_COIN);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; };

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetState(int state);
	void CreateItem(int itemType = ITEM_COIN_QUESTION_BRICK_COIN);
	CGameObject* SetUpItem(int itemType);
	~QuestionBrick();
};
