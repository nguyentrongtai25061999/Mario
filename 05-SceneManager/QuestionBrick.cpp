#include "QuestionBrick.h"
#include "Utils.h"
#include "Coin.h"
#include "PlayScene.h"
#include "Game.h"
#include "debug.h"
#include "Mario.h"
#include "MushRoom.h"

QuestionBrick::QuestionBrick(int tag) : CGameObject() {
	state = QUESTION_BRICK_NORMAL;
	this->tag = tag;
	this->item = NULL;
}
void QuestionBrick::Render() {
	int ani = -1;

	if (state == QUESTION_BRICK_NORMAL) {
		ani = QUESTION_BRICK_ANI_NORMAL;
	}
	if (state == QUESTION_BRICK_HIT) {
		ani = QUESTION_BRICK_ANI_HIT;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void QuestionBrick::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void QuestionBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	x += vx * dt;
	y += vy * dt;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (state == QUESTION_BRICK_HIT) {
		if (tag == ITEM_COIN_QUESTION_BRICK_COIN) {
			CreateItem(tag);
		}
		if (tag != ITEM_COIN_QUESTION_BRICK_COIN) {
			CreateItem(tag);
			DebugOut(L"Mushroom \n");
		}
	}
}

void QuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}
void QuestionBrick::CreateItem(int itemType) {
	this->obj = SetUpItem(itemType);
	if (this->obj == NULL) {
		//DebugOut(L"Mushroom 1 \n");
		return;
	}
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (dynamic_cast<CCoin*>(this->obj)) {
		CCoin* obj = dynamic_cast<CCoin*>(this->obj);
		obj->SetAppear(true);
		obj->SetPosition(x, y);
		obj->SetState(COIN_STATE_UP);
		obj->SetZIndex(-1);
		currentScene->AddObject(obj);
		//DebugOut(L"Coin was created \n");
	}
	if (dynamic_cast<CMushRoom*>(this->obj)) {
		CMushRoom* obj = dynamic_cast<CMushRoom*>(this->obj);
		obj->SetAppear(true);
		obj->SetPosition(x, y);
		obj->SetState(MUSHROOM_STATE_UP);
		obj->SetZIndex(-1);
		currentScene->AddObject(obj);
		//DebugOut(L"Mushroom was created \n");
	}
}
CGameObject* QuestionBrick::SetUpItem(int itemType) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	int ani_set_id = -1;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	// coin limmit
	if (totalItems >= 1) {
		totalItems--;
	}
	else {
		return NULL;
	}
	if (itemType == ITEM_COIN_QUESTION_BRICK_COIN) {
		obj = new CCoin(COIN_TYPE_INBRICK);
		ani_set_id = COIN_ANI_SET_ID;
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
	}
	if (itemType == ITEM_MUSHROOM_RED) {
			obj = new CMushRoom();
			ani_set_id = ITEM_MUSHROOM_ANI_SET_ID;
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj->SetAnimationSet(ani_set);
		}
	return obj;
}

void QuestionBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BRICK_NORMAL:
		vy = 0;
		break;
	}
}