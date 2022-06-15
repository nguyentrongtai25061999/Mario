#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->isTailFlying) {
			mario->vy = -0.065f;
			mario->ay = -0.005f;
			mario->isFlappingTailFlying = true;
		}
		else if (mario->GetLevel() == MARIO_LEVEL_TAIL && !mario->isOnPlatform)
			mario->isFlapping = true;
		else {
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_A:
		//mario->isReadyToHold = true;
		if (mario->GetLevel() == MARIO_LEVEL_TAIL && !mario->isSitting && !mario->isHolding)
			mario->SetState(MARIO_STATE_TAIL_ATTACK);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->isFlapping = false;
		mario->isFlappingTailFlying = false;
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_A:
		if (mario->isHolding) {
			mario->isReadyToHold = false;
			mario->isHolding = false;
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->isReadyToHold = true;
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else {
			mario->isReadyToHold = false;
			mario->isHolding = false;
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->isReadyToHold = true;
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		}
		else
		{
			mario->isReadyToHold = false;
			mario->isHolding = false;
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
	}
	else
		if (mario->getIsOnPlatForm() && !mario->isTuring) mario->SetState(MARIO_STATE_IDLE);
}