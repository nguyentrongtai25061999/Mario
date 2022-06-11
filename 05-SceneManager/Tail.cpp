#include "Tail.h"
#include "PlayScene.h"

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}
//
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	}

void CTail::Render() {
	RenderBoundingBox();
}
