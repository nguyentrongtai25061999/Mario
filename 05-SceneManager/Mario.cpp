#include <algorithm>
#include "debug.h"
#include "Block.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "Collision.h"
#include "MushRoom.h"
#include "Koopas.h"
#include "Tail.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	HandleMarioKicking();
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	//isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{

	if (!dynamic_cast<CBlock*>(e->obj)) {
		if (e->ny != 0 && e->obj->IsBlocking())
		{
			vy = 0;
			ay = MARIO_GRAVITY;
			if (e->ny < 0) {
				isOnPlatform = true;
				//DebugOut(L"BLOCK");
			}
		}
		else
		{
			if (e->nx != 0 && e->obj->IsBlocking())
			{
				vx = 0;
			}
		}
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<QuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CMushRoom*>(e->obj))
		OnCollisionWithMushRoom(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (goomba->tag == GOOMBA_RED) {
				goomba->SetTag(GOOMBA_RED_NORMAL);
			}
			else
				goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			//DebugOut(L"vx ,%f",vx);
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (goomba->tag == GOOMBA_RED)
					goomba->SetTag(GOOMBA_RED_NORMAL);
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	///*AddScore(this->x, this->y, 100, false);
	//AddCoin();*/
	coin++;
}
void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}
void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	QuestionBrick* qBrick = dynamic_cast<QuestionBrick*>(e->obj);

	// Hit from bottom
	if (e->ny > 0) {
		vy = 0;
		qBrick->SetState(QUESTION_BRICK_HIT);
	}
}
void CMario::OnCollisionWithMushRoom(LPCOLLISIONEVENT e)
{
	CMushRoom* mushRoom = dynamic_cast<CMushRoom*>(e->obj);
	mushRoom->Delete();
	SetLevel(MARIO_LEVEL_BIG);
}
void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e) {
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (e->nx != 0) {
		if (koopas->GetState() == KOOPAS_STATE_IN_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_UP) {
			if (isReadyToHold) {
				isHolding = true;
				koopas->SetIsBeingHeld(true);
			}
			else {
				SetState(MARIO_STATE_KICK);
				koopas->SetState(KOOPAS_STATE_SPINNING);
			}
		}
		else {
			HandleBasicMarioDie();
		}
	}
	if (e->ny > 0) {
		if (koopas->GetState() == KOOPAS_STATE_IN_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_UP) {
			SetState(MARIO_STATE_KICK);
			koopas->SetState(KOOPAS_STATE_SPINNING);
		}
		else {
			koopas->x = this->x + nx * 2;
			HandleBasicMarioDie();
		}
	}
	if (e->ny < 0) {
		CGame* game = CGame::GetInstance();

		vy = -MARIO_JUMP_DEFLECT_SPEED;
		if (koopas->GetState() == KOOPAS_STATE_WALKING) {
			if (koopas->tag == KOOPAS_GREEN_PARA) {
				koopas->SetTag(KOOPAS_GREEN);
			}
			else koopas->SetState(KOOPAS_STATE_IN_SHELL);
		}
		else if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
			koopas->SetState(KOOPAS_STATE_SPINNING);
			DebugOut(L"vy %f", koopas->vy);
			DebugOut(L"ay %f", koopas->ay);
		}
		else if (koopas->GetState() == KOOPAS_STATE_SPINNING) {
			koopas->SetState(KOOPAS_STATE_IN_SHELL);
		}
	}
}
void CMario::HandleBasicMarioDie() {

	if (level != MARIO_LEVEL_SMALL)
	{
		level -= 1;
		SetLevel(level);
		StartUntouchable();
		DebugOut(L">>> Mario TRANSFORM >>>%d \n", level);
	}
	else
	{
		DebugOut(L">>> Mario DIE >>> \n");
		SetState(MARIO_STATE_DIE);
	}
}
void CMario::HandleMarioKicking() {
	if (isKick) {
		if (GetTickCount64() - start_kicking > MARIO_KICKING_TIME) {
			StopKicking();
			SetState(MARIO_STATE_IDLE);
		}
	}
}
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP || isHolding || isKick) {
		if (nx > 0) {
			aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			if (isFlying) {
				aniId = MARIO_ANI_SMALL_FLY_RIGHT;
			}
			if (isHolding) {
				aniId = MARIO_ANI_SMALL_HOLD_RUNNING_RIGHT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_SMALL_KICKING_RIGHT;
			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
			if (isFlying) {
				aniId = MARIO_ANI_SMALL_FLY_LEFT;
			}
			if (isHolding) {
				aniId = MARIO_ANI_SMALL_HOLD_RUNNING_LEFT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_SMALL_KICKING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) {
					aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
					if (isKick) {
						aniId = MARIO_ANI_SMALL_KICKING_RIGHT;
					}
				}
				else {
					aniId = MARIO_ANI_SMALL_IDLE_LEFT;
					if (isKick) {
						aniId = MARIO_ANI_SMALL_KICKING_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_SMALL_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = MARIO_ANI_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X) {
					aniId = MARIO_ANI_SMALL_WALKING_RIGHT;
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
					if (isFlying) {
						aniId = MARIO_ANI_SMALL_FLY_RIGHT;
					}
				}
				if (isKick) {
					aniId = MARIO_ANI_SMALL_KICKING_RIGHT;
				}

			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_SMALL_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = MARIO_ANI_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = MARIO_ANI_SMALL_WALKING_LEFT;

				if (!isOnPlatform) {
					aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
					if (isFlying) {
						aniId = MARIO_ANI_SMALL_FLY_LEFT;
					}
				}
				if (isKick) {
					aniId = MARIO_ANI_SMALL_KICKING_LEFT;
				}
			}

	if (aniId == -1) aniId = MARIO_ANI_SMALL_IDLE_RIGHT;

	return aniId;
}
int CMario::GetAniIdTail() {
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MARIO_ANI_TAIL_RUNNING_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_RUNNING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_TAIL_WALKING_FAST_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_WALKING_FAST_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP || isHolding || isKick) {
		if (nx > 0) {
			aniId = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
			if (isTailFlying) {
				aniId = MARIO_ANI_TAIL_FLY_UP_RIGHT;
			}
			if (isFlapping)
				aniId = MARIO_ANI_TAIL_FLAPPING_RIGHT;
			else if (isFlappingTailFlying)
				aniId = MARIO_ANI_TAIL_FLY_FLAPPING_RIGHT;
			else if (isHolding) {
				aniId = MARIO_ANI_TAIL_HOLD_RUNNING_RIGHT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_TAIL_KICKING_RIGHT;
			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
			if (isTailFlying) {
				aniId = MARIO_ANI_TAIL_FLY_UP_LEFT;
			}
			if (isFlapping)
				aniId = MARIO_ANI_TAIL_FLAPPING_LEFT;
			else if (isFlappingTailFlying)
				aniId = MARIO_ANI_TAIL_FLY_FLAPPING_LEFT;
			else if (isHolding) {
				aniId = MARIO_ANI_TAIL_HOLD_RUNNING_LEFT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_TAIL_KICKING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = MARIO_ANI_TAIL_SITTING_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = MARIO_ANI_TAIL_IDLE_RIGHT;
				else aniId = MARIO_ANI_TAIL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_TAIL_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					aniId = MARIO_ANI_TAIL_RUNNING_RIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X) {
					aniId = MARIO_ANI_TAIL_WALKING_RIGHT;
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
					if (isTailFlying) {
						aniId = MARIO_ANI_TAIL_FLY_UP_RIGHT;
						if (isFlappingTailFlying)
							aniId = MARIO_ANI_TAIL_FLY_FLAPPING_RIGHT;
					}
					if (isFlapping)
						aniId = MARIO_ANI_TAIL_FLAPPING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_TAIL_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = MARIO_ANI_TAIL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = MARIO_ANI_TAIL_WALKING_LEFT;

				if (!isOnPlatform) {
					aniId = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
					if (isTailFlying) {
						aniId = MARIO_ANI_TAIL_FLY_UP_LEFT;
						if (isFlappingTailFlying)
							aniId = MARIO_ANI_TAIL_FLY_FLAPPING_LEFT;
					}
					if (isFlapping)
						aniId = MARIO_ANI_TAIL_FLAPPING_LEFT;
				}
			}

	if (aniId == -1) aniId = MARIO_ANI_TAIL_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
			else
				aniId = MARIO_ANI_BIG_RUNNING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
			else
				aniId = MARIO_ANI_BIG_WALKING_FAST_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP) {
		if (nx > 0) {
			aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
			if (isHolding) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_BIG_KICKING_RIGHT;

			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
			if (isHolding) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
			}
			else if (isKick)
			{
				aniId = MARIO_ANI_BIG_KICKING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0) {
				aniId = MARIO_ANI_BIG_SITTING_RIGHT;
			}
			else
				aniId = MARIO_ANI_BIG_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) {
					aniId = MARIO_ANI_BIG_IDLE_RIGHT;
					if (isKick) {
						aniId = MARIO_ANI_BIG_KICKING_RIGHT;
					}
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
					}
				}
				else {
					aniId = MARIO_ANI_BIG_IDLE_LEFT;
					if (isKick) {
						aniId = MARIO_ANI_BIG_KICKING_LEFT;
					}
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_BIG_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_RIGHT;
					}
				}
				else if (ax == MARIO_ACCEL_WALK_X) {
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
					}
					aniId = MARIO_ANI_BIG_WALKING_RIGHT;
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
					if (isFlying) {
						aniId = MARIO_ANI_BIG_FLY_RIGHT;
					}
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
					}
				}

				if (isKick) {
					aniId = MARIO_ANI_BIG_KICKING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_BIG_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					aniId = MARIO_ANI_BIG_RUNNING_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_LEFT;
					}
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
				{
					aniId = MARIO_ANI_BIG_WALKING_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
					}
				}

				if (!isOnPlatform) {
					aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
					if (isFlying) {
						aniId = MARIO_ANI_BIG_FLY_LEFT;
					}
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
					}
				}

				if (isKick) {
					aniId = MARIO_ANI_BIG_KICKING_LEFT;
				}
			}

	if (aniId == -1) aniId = MARIO_ANI_BIG_IDLE_RIGHT;

	return aniId;
}
void CMario::Render()
{
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = MARIO_ANI_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_TAIL)
		aniId = GetAniIdTail();

	if (state == MARIO_STATE_TRANSFORM) {
		if (nx > 0) {
			aniId = MARIO_ANI_TRANSFORM_SMALL_RIGHT;
		}
		else aniId = MARIO_ANI_TRANSFORM_SMALL_LEFT;
	}
	if (isSitting) {
		animation_set->at(aniId)->Render(x, y + 5);
	}
	else {
		animation_set->at(aniId)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_KICK:
		StartKicking();
		break;
	}
	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (level != MARIO_LEVEL_SMALL)
	{

		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (state == MARIO_STATE_SIT) {
			bottom = top + MARIO_BBOX_SIT_HEIGHT;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
	}
	level = l;
}

