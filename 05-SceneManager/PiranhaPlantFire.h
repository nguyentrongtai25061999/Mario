#pragma once
#include "PiranhaPlant.h"

#define PIRANHAPLANT_GREEN_TYPE					0
#define PIRANHAPLANT_RED_TYPE					1
#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_GREEN_BBOX_HEIGHT			24
#define PIRANHAPLANT_RED_BBOX_HEIGHT			32
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_ANI_RIGHT_UP			0
#define PIRANHAPLANT_ANI_RIGHT_DOWN			1
#define PIRANHAPLANT_ANI_LEFT_UP			2
#define PIRANHAPLANT_ANI_LEFT_DOWN			3
#define PIRANHAPLANT_STATE_INACTIVE			300


class PiranhaPlantFire :
	public PiranhaPlant
{
	float limitY = 0;
	bool Up = false;
	bool Right = false;
	int BBHeight = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
public:

	PiranhaPlantFire(int tag);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	void SetLimitY(float ly)
	{
		if (tag == 0)
			BBHeight = PIRANHAPLANT_GREEN_BBOX_HEIGHT;
		else BBHeight = PIRANHAPLANT_RED_BBOX_HEIGHT;
		limitY = ly - BBHeight;
	}
};
