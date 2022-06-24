#include "BreakableBrick.h"
#include "GameObject.h"
#include "PlayScene.h"

void BreakableBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}
