#include "Cursor.h"
#include <SpriteRenderer.h>
#include <InputManager.h>
#include <EngineScreen.h>
#include <EngineGeneralSound.h>
#include <EngineGeneralSoundPlayer.h>
#include "SelectionPortrait.h"
#include "SelectionFlag.h"
#include "CharacterSelectScene.h"
// Static Var
// Static Func

// constructer destructer
Cursor::Cursor()
	:cursorMoveRatio_(96.0f), currentCursor_(CURRENTCURSOR::MAX), mainSpriteRender_(nullptr), parentPortrait_(nullptr), isReady_(false), selectedCharacter_("")
{
	SetRenderOrder(10);
}

Cursor::~Cursor()
{
}

Cursor::Cursor(Cursor&& _other) noexcept
	:cursorMoveRatio_(96.0f), currentCursor_(CURRENTCURSOR::MAX), mainSpriteRender_(nullptr), parentPortrait_(nullptr), isReady_(false), selectedCharacter_("")
{
}

//member Func
void Cursor::LastCheck()
{
	AnimationInit();
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	if (nullptr == soundPlayer_)
	{
		EngineGeneralDebug::MsgBoxError("멤버 변수에 사운드 플레이어가 만들어지지 않았습니다.");
	}
	soundPlayer_->PlayCountReset(5);

}


void Cursor::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("Cursor.bmp");
	mainSpriteRender_->CreateSpriteAnimation("Blink1P", "Cursor.bmp", 0, 4, true, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("Blink2P", "Cursor.bmp", 5, 8, true, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("P1Selected", "Cursor.bmp", 0, 0, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("P2Selected", "Cursor.bmp", 5, 5, false, 0.017f);
	
}

void Cursor::PlayerInit()
{
	if ("p1Cursor" == GetName())
	{
		playerTag_ = "P1";
		if (false == InputManager::GetInputManager().IsButtonKey(playerTag_ + "_Right"))
		{
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Right", 'H', true);
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Left", 'F', true);
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Up", 'T', true);
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Down", 'G', true);
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_LP", 'Q');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_MP", 'W');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_HP", 'E');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_LK", 'A');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_MK", 'S');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_HK", 'D');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_SP", 'R');
			InputManager::GetInputManager().CreateInputKey("Debug", 'X'); // 판정박스 온오프용
		}
		SetPos({ 432,432 });
		mainSpriteRender_->ChangeSpriteAnimation("Blink1P");
	}



	if ("p2Cursor" == GetName())
	{
		playerTag_ = "P2";
		if (false == InputManager::GetInputManager().IsButtonKey(playerTag_ + "_Right"))
		{
			// 외부에서 "플레이어1" 인지 "플레이어2" 인지를 받은 다음 LastCheck()가 실행되거

			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Right", VK_RIGHT, true);
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Left", VK_LEFT, true);
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Up", VK_END, true); // 현재 이걸 VK_UP 으로 해놓으면 동시 입력에 문제가 생기는 중...
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_Down", VK_DOWN, true);
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_LP", 'U');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_MP", 'I');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_HP", 'O');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_LK", 'J');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_MK", 'K');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_HK", 'L');
			InputManager::GetInputManager().CreateInputKey(playerTag_ + "_SP", 'P');
		}
		SetPos({ 432,528 });
		mainSpriteRender_->ChangeSpriteAnimation("Blink2P");
	}
}




void Cursor::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}

void Cursor::UpdateBefore()
{
	soundPlayer_->PlayCountReset(1);
	if (432 == GetPos().iy())
	{
		switch (GetPos().ix())
		{
		case 432:
			currentCursor_ = CURRENTCURSOR::RYU;
			break;
		case 432 + 96:
			currentCursor_ = CURRENTCURSOR::EHONDA;
			break;
		case 432 + (96 * 2) :
			currentCursor_ = CURRENTCURSOR::BLANKA;
			break;
		case 432 + (96 * 3) :
			currentCursor_ = CURRENTCURSOR::GUILE;
			break;
		default:
			break;
		}
	}
	else if (432 + cursorMoveRatio_ == GetPos().iy())
	{
		switch (GetPos().ix())
		{
		case 432 - 96:
			currentCursor_ = CURRENTCURSOR::BOXER;
			break;
		case 432:
			currentCursor_ = CURRENTCURSOR::KEN;
			break;
		case 432 + 96 :
			currentCursor_ = CURRENTCURSOR::CHUNLI;
			break;
		case 432 + (96 * 2) :
			currentCursor_ = CURRENTCURSOR::ZANGIEF;
			break;
		case 432 + (96 * 3) :
			currentCursor_ = CURRENTCURSOR::DHALSIM;
			break;
		case 432 + (96 * 4) :
			currentCursor_ = CURRENTCURSOR::SAGAT;
			break;
		default:
			break;
		}
	}
	else if (432 + cursorMoveRatio_ * 2 == GetPos().iy())
	{
		switch (GetPos().ix())
		{
		case 432 - 96:
			currentCursor_ = CURRENTCURSOR::CLAW;
			break;
		case 432:
			currentCursor_ = CURRENTCURSOR::THAWK;
			break;
		case 432 + 96:
			currentCursor_ = CURRENTCURSOR::FEILONG;
			break;
			case 432 + (96 * 2) :
				currentCursor_ = CURRENTCURSOR::DEEJAY;
				break;
				case 432 + (96 * 3) :
					currentCursor_ = CURRENTCURSOR::CAMMY;
					break;
					case 432 + (96 * 4) :
						currentCursor_ = CURRENTCURSOR::DICTATOR;
						break;
					default:
						break;
		}
	}
}

void Cursor::Update()
{
	if (true == this->IsOn())
	{
		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_Right"))
		{
			soundPlayer_->PlayOverlap("CursorMove.wav", 0);
			if (GetPos().x < (432.0f + (cursorMoveRatio_ * 3)))
			{
				SetPos({ GetPos().x + cursorMoveRatio_, GetPos().y });
				return;
			}
			else if (GetPos().y > 432.0f && GetPos().x < (432.0f + (cursorMoveRatio_ * 4)))
			{
				SetPos({ GetPos().x + cursorMoveRatio_, GetPos().y });
				return;
			}
			else
			{
				if (GetPos().y > 432.0f)
				{
					SetPos({ GetPos().x - (cursorMoveRatio_ * 5), GetPos().y });
					return;
				}
				SetPos({ GetPos().x - (cursorMoveRatio_ * 3), GetPos().y });
				return;
			}

		}
		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_Left"))
		{
			soundPlayer_->PlayOverlap("CursorMove.wav", 0);
			if (GetPos().x > 432.0f)
			{
				SetPos({ GetPos().x - cursorMoveRatio_, GetPos().y });
				return;
			}
			else if (GetPos().y > 432.0f && GetPos().x >= 432.0f)
			{
				SetPos({ GetPos().x - cursorMoveRatio_, GetPos().y });
				return;
			}
			else
			{
				if (GetPos().y > 432.0f)
				{
					SetPos({ GetPos().x + (cursorMoveRatio_ * 5), GetPos().y });
					return;
				}
				SetPos({ GetPos().x + (cursorMoveRatio_ * 3), GetPos().y });
				return;
			}

		}
		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_Up"))
		{
			soundPlayer_->PlayOverlap("CursorMove.wav", 0);
			if (432.0f < GetPos().y && 432.0f - cursorMoveRatio_ < GetPos().x && 432.0f + cursorMoveRatio_ * 4 > GetPos().x)
			{
				SetPos({ GetPos().x, GetPos().y - cursorMoveRatio_ });
				return;
			}
			else if (432.0f + cursorMoveRatio_ < GetPos().y)
			{
				if (432.0f > GetPos().x || 432.0f + cursorMoveRatio_ * 3 < GetPos().x)
				{
					SetPos({ GetPos().x, GetPos().y - cursorMoveRatio_ });
					return;
				}

			}

		}
		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_Down"))
		{
			soundPlayer_->PlayOverlap("CursorMove.wav", 0);
			if (432.0f + cursorMoveRatio_ * 2 > GetPos().y)
			{
				SetPos({ GetPos().x, GetPos().y + cursorMoveRatio_ });
			}

		}
		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
		{
			soundPlayer_->PlayOverlap("CursorSelect.wav", 0);
			// 선택
			isReady_ = true;
			mainSpriteRender_->ChangeSpriteAnimation(playerTag_ + "Selected");
			switch (currentCursor_)
			{
			case CURRENTCURSOR::RYU:
				selectedCharacter_ = "Ryu";
				Off();
				break;
			case CURRENTCURSOR::EHONDA:
				selectedCharacter_ = "EHonda";
				Off();
				break;
			case CURRENTCURSOR::BLANKA:
				selectedCharacter_ = "Blanka";
				Off();
				break;
			case CURRENTCURSOR::GUILE:
				selectedCharacter_ = "Guile";
				Off();
				break;
			case CURRENTCURSOR::BOXER:
				selectedCharacter_ = "Boxer";
				Off();
				break;
			case CURRENTCURSOR::KEN:
				selectedCharacter_ = "Ken";
				Off();
				break;
			case CURRENTCURSOR::CHUNLI:
				selectedCharacter_ = "ChunLi";
				Off();
				break;
			case CURRENTCURSOR::ZANGIEF:
				selectedCharacter_ = "Zangief";
				Off();
				break;
			case CURRENTCURSOR::DHALSIM:
				selectedCharacter_ = "Dhalsim";
				Off();
				break;
			case CURRENTCURSOR::SAGAT:
				selectedCharacter_ = "Sagat";
				Off();
				break;
			case CURRENTCURSOR::CLAW:
				selectedCharacter_ = "Claw";
				Off();
				break;
			case CURRENTCURSOR::THAWK:
				selectedCharacter_ = "THawk";
				Off();
				break;
			case CURRENTCURSOR::FEILONG:
				selectedCharacter_ = "FeiLong";
				Off();
				break;
			case CURRENTCURSOR::DEEJAY:
				selectedCharacter_ = "DeeJay";
				Off();
				break;
			case CURRENTCURSOR::CAMMY:
				selectedCharacter_ = "Cammy";
				Off();
				break;
			case CURRENTCURSOR::DICTATOR:
				selectedCharacter_ = "Dictator";
				Off();
				break;
			case CURRENTCURSOR::MAX:
				break;
			default:
				break;
			}

		}
	}
	
}

void Cursor::UpdateAfter()
{
	// 현재 좌표위치 따서 그 위치에 맞는 스테이트 작성해 연동된 포트레이트에 알려주기
	switch (currentCursor_)
	{
	case CURRENTCURSOR::RYU:
		parentPortrait_->ChangePortrait(playerTag_ + "Ryu");
		parentFlag_->ChangeFlagAndPos("Japan", { 1056,204 });
		break;
	case CURRENTCURSOR::EHONDA:
		parentPortrait_->ChangePortrait(playerTag_ + "Ehonda");
		parentFlag_->ChangeFlagAndPos("Japan", { 1008,108 });
		break;
	case CURRENTCURSOR::BLANKA:
		parentPortrait_->ChangePortrait(playerTag_ + "Blanka");
		parentFlag_->ChangeFlagAndPos("Brazil", { 99,300 });
		break;
	case CURRENTCURSOR::GUILE:
		parentPortrait_->ChangePortrait(playerTag_ + "Guile");
		parentFlag_->ChangeFlagAndPos("USA", { 240,252 });
		break;
	case CURRENTCURSOR::BOXER:
		parentPortrait_->ChangePortrait(playerTag_ + "Boxer");
		parentFlag_->ChangeFlagAndPos("USA", { 96,108 });
		break;
	case CURRENTCURSOR::KEN:
		parentPortrait_->ChangePortrait(playerTag_ + "Ken");
		parentFlag_->ChangeFlagAndPos("USA", { 432,131 });
		break;
	case CURRENTCURSOR::CHUNLI:
		parentPortrait_->ChangePortrait(playerTag_ + "ChunLi");
		parentFlag_->ChangeFlagAndPos("China", { 1056,300 });
		break;
	case CURRENTCURSOR::ZANGIEF:
		parentPortrait_->ChangePortrait(playerTag_ + "Zangief");
		parentFlag_->ChangeFlagAndPos("USSR", { 768,108 });
		break;
	case CURRENTCURSOR::DHALSIM:
		parentPortrait_->ChangePortrait(playerTag_ + "Dhalsim");
		parentFlag_->ChangeFlagAndPos("India", { 624,300 });
		break;
	case CURRENTCURSOR::SAGAT:
		parentPortrait_->ChangePortrait(playerTag_ + "Sagat");
		parentFlag_->ChangeFlagAndPos("Thailand", { 768,300 });
		break;
	case CURRENTCURSOR::CLAW:
		parentPortrait_->ChangePortrait(playerTag_ + "Claw");
		parentFlag_->ChangeFlagAndPos("Spain", { 480,300 });
		break;
	case CURRENTCURSOR::THAWK:
		parentPortrait_->ChangePortrait(playerTag_ + "THawk");
		parentFlag_->ChangeFlagAndPos("Mexico", { 96,204 });
		break;
	case CURRENTCURSOR::FEILONG:
		parentPortrait_->ChangePortrait(playerTag_ + "FeiLong");
		parentFlag_->ChangeFlagAndPos("Hongkong", { 912,276 });
		break;
	case CURRENTCURSOR::DEEJAY:
		parentPortrait_->ChangePortrait(playerTag_ + "DeeJay");
		parentFlag_->ChangeFlagAndPos("Jamaica", { 381,228 });
		break;
	case CURRENTCURSOR::CAMMY:
		parentPortrait_->ChangePortrait(playerTag_ + "Cammy");
		parentFlag_->ChangeFlagAndPos("England", { 624,204 });
		break;
	case CURRENTCURSOR::DICTATOR:
		parentPortrait_->ChangePortrait(playerTag_ + "Dictator");
		parentFlag_->ChangeFlagAndPos("Thailand", { 768,300 });
		break;
	case CURRENTCURSOR::MAX:
		break;
	default:
		break;
	}


}

