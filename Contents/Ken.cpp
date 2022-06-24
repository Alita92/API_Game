#include "Ken.h"
#include "Projectile.h"
#include <EngineScreen.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <EngineGeneralTime.h>
#include <SpriteRenderer.h>
#include <EngineGeneralSound.h>
#include <EngineGeneralSoundPlayer.h>
#include "ContentsENUM.h"
#include <EngineGeneralMath.h>
#include <GameCollision.h>
#include "StageRyu.h"
#include "FightScene.h"
#include "FightEffects.h"


Ken::Ken()
	: stateType_(KenState::MAX), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr)
{
	hp_ = 180;
	originalHp_ = 180;
	maxJumpHeight_ = 220.0f;
	moveSpeed_ = 300.0f;
	SetRenderOrder(10);
	jumpForce_ = 1300.0f;
}

Ken::~Ken()
{
}

Ken::Ken(Ken&& _other) noexcept
	: stateType_(_other.stateType_), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr)
{
}

void Ken::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();

	if (true == isDebugOn_)
	{
		// *** 하기는 판정박스 디버그용 랜더링입니다.
		// bodyBox_->DebugRender();
		//collisionBox_->StartDebugRender();
		//hitBoxHead_->StartDebugRender();
		hitBoxTorso_->StartDebugRender();
		//hitBoxLeg_->StartDebugRender();
		// guardRangeBox_->StartDebugRender();
		//guardBox_->StartDebugRender();
		//crouchGuardBox_->StartDebugRender();
		strikeBox_->StartDebugRender();
		strikeBoxOverHead_->StartDebugRender();
		strikeBoxLow_->StartDebugRender();
		// ->StartDebugRender();
	}
}

void Ken::ToggleDebug()
{
	if (InputManager::GetInputManager().IsButtonDown("Debug"))
	{
		if (true == isDebugOn_)
		{
			isDebugOn_ = false;
			return;
		}
		isDebugOn_ = true;
		return;
	}

	

}


void Ken::ToggleAI()
{
	if (true == InputManager::GetInputManager().IsButtonDown("P1_AIToggle"))
	{
		if (playerTag_ == "P1")
		{
			if (true == isAIOn_)
			{
				isAIOn_ = false;
				return;
			}
			isAIOn_ = true;
			return;
		}
		if (playerTag_ == "P2")
		{
			return;
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown("P2_AIToggle"))
	{
		if (playerTag_ == "P1")
		{
			return;
		}
		if (playerTag_ == "P2")
		{
			if (true == isAIOn_)
			{
				isAIOn_ = false;
				return;
			}
			isAIOn_ = true;
			return;
		}
	}

}

void Ken::KeyInit()
{
	if ("P1" == GetName())
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

		dirstate_ = ACTORDIR::LOOK_RIGHT;

		collisionBox_ = CreateCollision(ST2GROUP::PLAYER1COLLISION, ColliderType::RECT);

		bodyBox_ = CreateCollision(ST2GROUP::PLAYER1BODY, ColliderType::RECT);

		//hitBoxHead_ = CreateCollision(ST2GROUP::PLAYER1HIT, ColliderType::RECT);
		hitBoxTorso_ = CreateCollision(ST2GROUP::PLAYER1HIT, ColliderType::RECT);
		//hitBoxLeg_ = CreateCollision(ST2GROUP::PLAYER1HIT, ColliderType::RECT);

		strikeBox_ = CreateCollision(ST2GROUP::PLAYER1STRIKE, ColliderType::RECT);
		strikeBoxOverHead_ = CreateCollision(ST2GROUP::PLAYER1STRIKEOVERHEAD, ColliderType::RECT);
		strikeBoxLow_ = CreateCollision(ST2GROUP::PLAYER1STRIKELOW, ColliderType::RECT);

		guardRangeBox_ = CreateCollision(ST2GROUP::PLAYER1GUARDRANGE, ColliderType::RECT);

		guardBox_ = CreateCollision(ST2GROUP::PLAYER1GUARD, ColliderType::RECT);
		crouchGuardBox_ = CreateCollision(ST2GROUP::PLAYER1CROUCHGUARD, ColliderType::RECT);

		enemyBody_ = ST2GROUP::PLAYER2BODY;
		enemyCollision_ = ST2GROUP::PLAYER2COLLISION;
		enemyHit_ = ST2GROUP::PLAYER2HIT;
		enemyStrike_ = ST2GROUP::PLAYER2STRIKE;
		enemyStrikeOverHead_ = ST2GROUP::PLAYER2STRIKEOVERHEAD;
		enemyStrikeLow_ = ST2GROUP::PLAYER2STRIKELOW;
		enemyGuardRange_ = ST2GROUP::PLAYER2GUARDRANGE;
		enemyGuard_ = ST2GROUP::PLAYER2GUARD;
		enemyCrouchGuard_ = ST2GROUP::PLAYER2CROUCHGUARD;
		enemyStrikeProjectile_ = ST2GROUP::PLAYER2PROJECTILE;
		otherPlayerTag_ = "P2";
	}

	if ("P2" == GetName())
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

		dirstate_ = ACTORDIR::LOOK_LEFT;

		// 판정 박스의 피벗은 추후 조건문을 통해 1P, 2P를 구분해 초기화해줘야 합니다.
		collisionBox_ = CreateCollision(ST2GROUP::PLAYER2COLLISION, ColliderType::RECT);

		bodyBox_ = CreateCollision(ST2GROUP::PLAYER2BODY, ColliderType::RECT);


		//hitBoxHead_ = CreateCollision(ST2GROUP::PLAYER2HIT, ColliderType::RECT);
		hitBoxTorso_ = CreateCollision(ST2GROUP::PLAYER2HIT, ColliderType::RECT);
		//hitBoxLeg_ = CreateCollision(ST2GROUP::PLAYER2HIT, ColliderType::RECT);

		strikeBox_ = CreateCollision(ST2GROUP::PLAYER2STRIKE, ColliderType::RECT);
		strikeBoxOverHead_ = CreateCollision(ST2GROUP::PLAYER2STRIKEOVERHEAD, ColliderType::RECT);
		strikeBoxLow_ = CreateCollision(ST2GROUP::PLAYER2STRIKELOW, ColliderType::RECT);

		guardRangeBox_ = CreateCollision(ST2GROUP::PLAYER2GUARDRANGE, ColliderType::RECT);

		guardBox_ = CreateCollision(ST2GROUP::PLAYER2GUARD, ColliderType::RECT);
		crouchGuardBox_ = CreateCollision(ST2GROUP::PLAYER2CROUCHGUARD, ColliderType::RECT);

		enemyBody_ = ST2GROUP::PLAYER1BODY;
		enemyCollision_ = ST2GROUP::PLAYER1COLLISION;
		enemyHit_ = ST2GROUP::PLAYER1HIT;
		enemyStrike_ = ST2GROUP::PLAYER1STRIKE;
		enemyStrikeOverHead_ = ST2GROUP::PLAYER1STRIKEOVERHEAD;
		enemyStrikeLow_ = ST2GROUP::PLAYER1STRIKELOW;
		enemyGuardRange_ = ST2GROUP::PLAYER1GUARDRANGE;
		enemyGuard_ = ST2GROUP::PLAYER1GUARD;
		enemyCrouchGuard_ = ST2GROUP::PLAYER1CROUCHGUARD;
		enemyStrikeProjectile_ = ST2GROUP::PLAYER1PROJECTILE;
		otherPlayerTag_ = "P1";
	}

	strikeBoxPivot_ = { 0, 0 };
	strikeBox_->SetPivot({ 0,0 });
	strikeBox_->SetSize({ 10,10 });
	strikeBox_->SetCameraEffectOn();
	strikeBoxOverHeadPivot_ = { 0, 0 };
	strikeBoxOverHead_->SetPivot({ 0,0 });
	strikeBoxOverHead_->SetSize({ 10,10 });
	strikeBoxOverHead_->SetCameraEffectOn();
	strikeBoxLowPivot_ = { 0, 0 };
	strikeBoxLow_->SetPivot({ 0,0 });
	strikeBoxLow_->SetSize({ 10,10 });
	strikeBoxLow_->SetCameraEffectOn();

	bodyBox_->SetPivot({ 0,15 });
	bodyBox_->SetSize({ 120,220 });
	bodyBox_->SetCameraEffectOn();
	collisionBox_->SetPivot({ 0,15 });
	collisionBox_->SetSize({ 90,220 });
	collisionBox_->SetCameraEffectOn();
	hitBoxTorsoPivot_ = { -10, 15 };
	hitBoxTorso_->SetSize({ 100,210 });
	hitBoxTorso_->SetCameraEffectOn();
	guardRangeBoxPivot_ = { 200, 0 };
	guardRangeBox_->SetSize({ 600,1000 });
	guardRangeBox_->SetCameraEffectOn();

	guardBoxPivot_ = { 0, 1000 };
	guardBox_->SetSize({ 0,0 });
	guardBox_->SetCameraEffectOn();

	crouchGuardBoxPivot_ = { 0,1000 };
	crouchGuardBox_->SetSize({ 0,0 });
	crouchGuardBox_->SetCameraEffectOn();
}

void Ken::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("SpritesKen.bmp");

	mainSpriteRender_->CreateSpriteAnimation("IdleL", "SpritesKen.bmp", 0, 3, true, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("IdleR", "SpritesKen.bmp", 0 + 200, 3 + 200, true, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("WalkL", "SpritesKen.bmp", 4, 8, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("WalkR", "SpritesKen.bmp", 4 + 200, 8 + 200, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("CrouchL", "SpritesKen.bmp", 24, 24, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchR", "SpritesKen.bmp", 24 + 200, 24 + 200, false, 0.051f);


	mainSpriteRender_->CreateSpriteAnimation("GuardL", "SpritesKen.bmp", 25, 25, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardL", "SpritesKen.bmp", 26, 26, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("GuardR", "SpritesKen.bmp", 25 + 200, 25 + 200, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardR", "SpritesKen.bmp", 26 + 200, 26 + 200, true, 0.1f);



	// 수직점프 애니메이션 설정
	mainSpriteRender_->CreateSpriteAnimation("JumpL", "SpritesKen.bmp", 9, 14, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("JumpR", "SpritesKen.bmp", 9 + 200, 14 + 200, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("JumpL", 9, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpL", 14, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpR", 9 + 200, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpR", 14 + 200, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("JumpFrontL", "SpritesKen.bmp", 16, 22, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpFrontR", "SpritesKen.bmp", 16 + 200, 22 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 16, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 17, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 21, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 22, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 16 + 200, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 17 + 200, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 21 + 200, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 22 + 200, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("JumpBackL", "SpritesKen.bmp", 16, 22, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpBackR", "SpritesKen.bmp", 16 + 200, 22 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 16, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 17, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 21, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 22, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 16 + 200, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 17 + 200, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 21 + 200, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 22 + 200, 0.204f);

	// 피격 & 다운 & 리커버리 & K.O. & 승리포즈
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightL", "SpritesKen.bmp", 164, 165, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightR", "SpritesKen.bmp", 164 + 200, 165 + 200, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyL", "SpritesKen.bmp", 164, 167, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyR", "SpritesKen.bmp", 164 + 200, 167 + 200, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownLightL", "SpritesKen.bmp", 161, 161, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownLightR", "SpritesKen.bmp", 161 + 200, 161 + 200, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyL", "SpritesKen.bmp", 160, 163, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyR", "SpritesKen.bmp", 160 + 200, 163 + 200, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchL", "SpritesKen.bmp", 169, 169, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchR", "SpritesKen.bmp", 169 + 200, 169 + 200, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpL", "SpritesKen.bmp", 170, 171, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpR", "SpritesKen.bmp", 170 + 200, 171 + 200, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownL", "SpritesKen.bmp", 172, 173, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownR", "SpritesKen.bmp", 172 + 200, 173 + 200, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownL", "SpritesRyu.bmp", 172, 173, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownR", "SpritesRyu.bmp", 172 + 210, 173 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownL", "SpritesKen.bmp", 196, 199, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownR", "SpritesKen.bmp", 196 + 200, 199 + 200, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("DownL", "SpritesKen.bmp", 170, 173, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("DownR", "SpritesKen.bmp", 170 + 200, 173 + 200, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BurnL", "SpritesKen.bmp", 132, 134, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("BurnR", "SpritesKen.bmp", 132 + 200, 134 + 200, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("RecoveryL", "SpritesKen.bmp", 174, 177, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("RecoveryR", "SpritesKen.bmp", 174 + 200, 177 + 200, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("GetUpL", "SpritesKen.bmp", 173, 177, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("GetUpR", "SpritesKen.bmp", 173 + 200, 177 + 200, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 174, 0.252f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 174 + 200, 0.252f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 175, 0.152f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 175 + 200, 0.152f);

	mainSpriteRender_->CreateSpriteAnimation("KOL", "SpritesKen.bmp", 180, 185, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("KOR", "SpritesKen.bmp", 180 + 200, 185 + 200, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("KOL", 185, 0.85f);
	mainSpriteRender_->SetChangeFrame("KOR", 185 + 200, 0.85f);

	mainSpriteRender_->CreateSpriteAnimation("R1WinL", "SpritesKen.bmp", 189, 191, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("R1WinR", "SpritesKen.bmp", 189 + 200, 191 + 200, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinL", "SpritesKen.bmp", 192, 194, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinR", "SpritesKen.bmp", 192 + 200, 194 + 200, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("StandDelayL", "SpritesKen.bmp", 9, 9, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("StandDelayR", "SpritesKen.bmp", 9 + 200, 9 + 200, false, 0.085f); // 5프레임 지연

	mainSpriteRender_->CreateSpriteAnimation("LandDelayL", "SpritesKen.bmp", 9, 9, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LandDelayR", "SpritesKen.bmp", 9 + 200, 9 + 200, false, 0.034f); // 2프레임 지연
	
	// 기본기
	mainSpriteRender_->CreateSpriteAnimation("LPL", "SpritesKen.bmp", 27, 29, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("LPR", "SpritesKen.bmp", 27 + 200, 29 + 200, false, 0.05f);
	mainSpriteRender_->SetChangeFrame("LPL", 27, 0.051f);
	mainSpriteRender_->SetChangeFrame("LPR", 27 + 200, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("LP_CloseL", "SpritesKen.bmp", 35, 37, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CloseR", "SpritesKen.bmp", 35 + 200, 37 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 35, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 37, 0.085f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 35 + 200, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 37 + 200, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LP_JumpL", "SpritesKen.bmp", 94, 95, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_JumpR", "SpritesKen.bmp", 94 + 200, 95 + 200, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalL", "SpritesKen.bmp", 94, 95, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalR", "SpritesKen.bmp", 94 + 200, 95 + 200, false, 0.034f);


	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchL", "SpritesKen.bmp", 71, 73, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchR", "SpritesKen.bmp", 71 + 200, 73 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchL", 72, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchL", 73, 0.085f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchR", 72 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchR", 73 + 200, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("MPL", "SpritesKen.bmp", 31, 33, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MPR", "SpritesKen.bmp", 31 + 200, 33 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MPL", 33, 0.102f);
	mainSpriteRender_->SetChangeFrame("MPR", 33 + 200, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("MP_JumpL", "SpritesKen.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_JumpR", "SpritesKen.bmp", 94 + 200, 96 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_JumpL", 95, 0.3f);
	mainSpriteRender_->SetChangeFrame("MP_JumpR", 95 + 200, 0.3f);
	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalL", "SpritesKen.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalR", "SpritesKen.bmp", 94 + 200, 96 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_DiagonalL", 95, 0.3f);
	mainSpriteRender_->SetChangeFrame("MP_DiagonalR", 95 + 200, 0.3f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CloseL", "SpritesKen.bmp", 39, 41, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CloseR", "SpritesKen.bmp", 39 + 200, 41 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CloseL", 40, 0.153f);
	mainSpriteRender_->SetChangeFrame("MP_CloseL", 41, 0.170f);
	mainSpriteRender_->SetChangeFrame("MP_CloseR", 40 + 200, 0.153f);
	mainSpriteRender_->SetChangeFrame("MP_CloseR", 41 + 200, 0.170f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchL", "SpritesKen.bmp", 74, 76, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchR", "SpritesKen.bmp", 74 + 200, 76 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 75, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 76, 0.085f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 75 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 76 + 200, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HPL", "SpritesKen.bmp", 31, 33, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HPR", "SpritesKen.bmp", 31 + 200, 33 + 200, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("HPL", 33, 0.374f);
	mainSpriteRender_->SetChangeFrame("HPR", 33 + 200, 0.374f);

	mainSpriteRender_->CreateSpriteAnimation("HP_JumpL", "SpritesKen.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_JumpR", "SpritesKen.bmp", 94 + 200, 96 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_JumpL", 95, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_JumpR", 95 + 200, 0.34f);
	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalL", "SpritesKen.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalR", "SpritesKen.bmp", 94 + 200, 96 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalL", 95, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalR", 95 + 200, 0.34f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CloseL", "SpritesKen.bmp", 45, 48, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CloseR", "SpritesKen.bmp", 45 + 200, 48 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 46, 0.034f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 47, 0.102f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 48, 0.391f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 46 + 200, 0.034f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 47 + 200, 0.102f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 48 + 200, 0.391f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchL", "SpritesKen.bmp", 78, 82, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchR", "SpritesKen.bmp", 78 + 200, 82 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 79, 0.051f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 80, 0.136f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 81, 0.204f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 82, 0.187f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 79 + 200, 0.051f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 80 + 200, 0.136f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 81 + 200, 0.204f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 82 + 200, 0.187f);

	mainSpriteRender_->CreateSpriteAnimation("LKL", "SpritesKen.bmp", 50, 52, false, 0.119f);
	mainSpriteRender_->CreateSpriteAnimation("LKR", "SpritesKen.bmp", 50 + 200, 52 + 200, false, 0.119f);
	mainSpriteRender_->SetChangeFrame("LKL", 51, 0.136f);
	mainSpriteRender_->SetChangeFrame("LKL", 52, 0.085f);
	mainSpriteRender_->SetChangeFrame("LKR", 51 + 200, 0.136f);
	mainSpriteRender_->SetChangeFrame("LKR", 52 + 200, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_JumpL", "SpritesKen.bmp", 96, 98, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpR", "SpritesKen.bmp", 96 + 200, 98 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_JumpL", 97, 0.68f);
	mainSpriteRender_->SetChangeFrame("LK_JumpR", 97 + 200, 0.68f);

	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalL", "SpritesKen.bmp", 103, 103, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalR", "SpritesKen.bmp", 103 + 200, 103 + 200, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_JumpFrontL", "SpritesKen.bmp", 102, 103, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpFrontR", "SpritesKen.bmp", 102 + 200, 103 + 200, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpBackL", "SpritesKen.bmp", 102, 103, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpBackR", "SpritesKen.bmp", 102 + 200, 103 + 200, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CloseL", "SpritesKen.bmp", 58, 60, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CloseR", "SpritesKen.bmp", 58 + 200, 60 + 200, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("LK_CloseL", 60, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CloseR", 60 + 200, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchL", "SpritesKen.bmp", 83, 85, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchR", "SpritesKen.bmp", 83 + 200, 85 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 84, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 85, 0.085f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 84 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 85 + 200, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("MKL", "SpritesKen.bmp", 50, 52, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("MKR", "SpritesKen.bmp", 50 + 200, 52 + 200, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("MKL", 52, 0.119f);
	mainSpriteRender_->SetChangeFrame("MKR", 52 + 200, 0.119f);

	mainSpriteRender_->CreateSpriteAnimation("MK_JumpL", "SpritesKen.bmp", 96, 98, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_JumpR", "SpritesKen.bmp", 96 + 200, 98 + 200, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_JumpL", 97, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_JumpR", 97 + 200, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalL", "SpritesKen.bmp", 104, 106, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalR", "SpritesKen.bmp", 104 + 200, 106 + 200, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalL", 105, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalR", 105 + 200, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CloseL", "SpritesKen.bmp", 62, 64, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CloseR", "SpritesKen.bmp", 62 + 200, 64 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CloseL", 63, 0.102f);
	mainSpriteRender_->SetChangeFrame("MK_CloseL", 64, 0.136f);
	mainSpriteRender_->SetChangeFrame("MK_CloseR", 63 + 200, 0.102f);
	mainSpriteRender_->SetChangeFrame("MK_CloseR", 64 + 200, 0.136f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchL", "SpritesKen.bmp", 86, 88, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchR", "SpritesKen.bmp", 86 + 200, 88 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 87, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 88, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 87 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 88 + 200, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HKL", "SpritesKen.bmp", 53, 57, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HKR", "SpritesKen.bmp", 53 + 200, 57 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("HKL", 54, 0.068f);
	mainSpriteRender_->SetChangeFrame("HKL", 55, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKL", 56, 0.153f);
	mainSpriteRender_->SetChangeFrame("HKL", 57, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKR", 54 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("HKR", 55 + 200, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKR", 56 + 200, 0.153f);
	mainSpriteRender_->SetChangeFrame("HKR", 57 + 200, 0.136f);

	mainSpriteRender_->CreateSpriteAnimation("HK_JumpL", "SpritesKen.bmp", 98, 102, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_JumpR", "SpritesKen.bmp", 98 + 200, 102 + 200, false, 0.034f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 99, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 100, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 101, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 99 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 100 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 101 + 200, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalL", "SpritesKen.bmp", 104, 106, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalR", "SpritesKen.bmp", 104 + 200, 106 + 200, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("HK_DiagonalL", 105, 0.119f);
	mainSpriteRender_->SetChangeFrame("HK_DiagonalR", 105 + 200, 0.119f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CloseL", "SpritesKen.bmp", 67, 70, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CloseR", "SpritesKen.bmp", 67 + 200, 70 + 200, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 69, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 70, 0.187f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 69 + 200, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 70 + 200, 0.187f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchL", "SpritesKen.bmp", 88, 93, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchR", "SpritesKen.bmp", 88 + 200, 93 + 200, false, 0.034f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 90, 0.102f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 91, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 92, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 93, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 90 + 200, 0.102f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 91 + 200, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 92 + 200, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 93 + 200, 0.136f);

	// 필살기
	// 약 파동권
	mainSpriteRender_->CreateSpriteAnimation("HadoukenL", "SpritesKen.bmp", 107, 110, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenR", "SpritesKen.bmp", 107 + 200, 110 + 200, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("HadoukenL", 108, 0.153f);
	mainSpriteRender_->SetChangeFrame("HadoukenR", 108 + 200, 0.153f);
	mainSpriteRender_->SetChangeFrame("HadoukenL", 110, 0.663f);
	mainSpriteRender_->SetChangeFrame("HadoukenR", 110 + 200, 0.663f);

	// 강 승룡권
	mainSpriteRender_->CreateSpriteAnimation("ShoryukenL", "SpritesKen.bmp", 139, 144, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("ShoryukenR", "SpritesKen.bmp", 139 + 200, 144 + 200, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("ShoryukenL", 142, 0.238f);
	mainSpriteRender_->SetChangeFrame("ShoryukenL", 143, 0.221f);
	mainSpriteRender_->SetChangeFrame("ShoryukenL", 144, 0.085f);
	mainSpriteRender_->SetChangeFrame("ShoryukenR", 142 + 200, 0.238f);
	mainSpriteRender_->SetChangeFrame("ShoryukenR", 143 + 200, 0.221f);
	mainSpriteRender_->SetChangeFrame("ShoryukenR", 144 + 200, 0.085f);

	// 강 용권선풍각
	mainSpriteRender_->CreateSpriteAnimation("TatsumakiL", "SpritesKen.bmp", 117, 129, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("TatsumakiR", "SpritesKen.bmp", 117 + 200, 129 + 200, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 117, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 118, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 128, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 129, 0.119f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 117 + 200, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 118 + 200, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 128 + 200, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 129 + 200, 0.119f);


	mainSpriteRender_->CreateSpriteAnimation("ThrowL", "SpritesKen.bmp", 145 + 200, 149 + 200, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("ThrowR", "SpritesKen.bmp", 145, 149, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("ThrowL", 145 + 200, 0.306f);
	mainSpriteRender_->SetChangeFrame("ThrowR", 145, 0.306f);

}

void Ken::LastCheck()
{
	AnimationInit();
	dirstate_ = ACTORDIR::LOOK_LEFT;
	ChangeState(KenState::Idle);
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	soundPlayer_->PlayCountReset(1);
	voicePlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	voicePlayer_->PlayCountReset(1);
}

void Ken::ChangeState(KenState _stateType)
{
	if (_stateType == KenState::NONE)
	{
		return;
	}
	stateType_ = _stateType;

	switch (stateType_)
	{
	case KenState::Idle:
		StateName = "Idle";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Walk:
		StateName = "Walk";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Crouch:
		StateName = "Crouch";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Jump:
		StateName = "Jump";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::JumpFront:
		StateName = "JumpFront";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::JumpBack:
		StateName = "JumpBack";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Guard:
		StateName = "Guard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::CrouchGuard:
		StateName = "CrouchGuard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HitUpperLight:
		StateName = "HitUpperLight";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HitUpperHeavy:
		StateName = "HitUpperHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HitDownLight:
		StateName = "HitDownLight";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HitDownHeavy:
		StateName = "HitDownHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HitWhenCrouch:
		StateName = "HitWhenCrouch";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HitWhenJump:
		StateName = "HitWhenJump";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HitWhenJumpDown:
		StateName = "HitWhenJumpDown";
		JumpDir = float4::DOWN * jumpForce_;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Down:
		StateName = "Down";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Recovery:
		StateName = "Recovery";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::GetUp:
		StateName = "GetUp";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::KO:
		StateName = "KO";
		JumpDir = float4::UP * jumpForce_ * 0.5f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::JumpKO:
		StateName = "HitWhenJump";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::R1Win:
		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			StateName = "R1Win";
			mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		}
		break;
	case KenState::MatchPointWin:
		StateName = "MatchPointWin";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::StandDelay:
		StateName = "StandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LandDelay:
		StateName = "LandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LP:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::LP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::LP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::MP:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::MP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::HP:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::HP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::HP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::HP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::HP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LK:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "LK_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::LK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::LK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MK:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "MK_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::MK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::MK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HK:
		strikeBox_->SetCollisionCount(1);
		StateName = "HK";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HK_Close:
		strikeBox_->SetCollisionCount(2);
		StateName = "HK_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "HK_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::HK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::HK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::HK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;


	case KenState::Hadouken:
		if (nullptr != projectile_)
		{
			break;
		}
		isShooting_ = false;
		strikeBox_->SetCollisionCount(0);
		StateName = "Hadouken";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Shoryuken:
		strikeBox_->SetCollisionCount(1);
		StateName = "Shoryuken";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Ken_Shoryuken.mp3", 0);
		break;
	case KenState::Tatsumaki:
		strikeBox_->SetCollisionCount(1);
		StateName = "Tatsumaki";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Ken_Tatsumaki.mp3", 0);
		break;
	case KenState::Throw:
		StateName = "Throw";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Ken_Throw.mp3", 0);
		break;
	case KenState::BeThrown:
		StateName = "BeThrown";
		JumpDir = float4::UP * jumpForce_ * 0.1f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::BeThrownDown:
		StateName = "BeThrownDown";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case KenState::Burn:
		StateName = "Burn";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case KenState::AIIdle:
		StateName = "Idle";
		mainSpriteRender_->ChangeSpriteAnimation("Idle" + GetDirString());
		break;
	case KenState::AIWalk:
		StateName = "Walk";
		mainSpriteRender_->ChangeSpriteAnimation("Walk" + GetDirString());
		break;
	case KenState::AICrouch:
		StateName = "Crouch";
		mainSpriteRender_->ChangeSpriteAnimation("Crouch" + GetDirString());
		break;
	case KenState::AIJump:
		StateName = "Jump";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation("Jump" + GetDirString());
		break;
	case KenState::AIJumpFront:
		StateName = "JumpFront";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation("JumpFront" + GetDirString());
		break;
	case KenState::AIJumpBack:
		StateName = "JumpBack";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation("JumpBack" + GetDirString());
		break;
	default:
		break;
	}
}

void Ken::UpdateBefore()
{
	ToggleDebug();
	ToggleAI();
	if (this->hp_ <= 0)
	{
		voicePlayer_->PlayOverlap("Ken_KO.mp3", 0);
		if (true == mainSpriteRender_->IsCurSpriteAnimationName("BeThrownDown" + GetDirString()))
		{
			LeftRightCheck();
		}
		if (450.0f > GetPos().y)
		{
			return ChangeState(KenState::JumpKO);

		}
		return ChangeState(KenState::KO);
	}
}

std::string Ken::GetDirString() // 업데이트로 체크된 "지향 방향" 을 측정해 글자를 리턴
{
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		return "L";
	case ACTORDIR::LOOK_LEFT:
		return "R";
	default:
		EngineGeneralDebug::GameDestroyer();
		break;
	}
	EngineGeneralDebug::GameDestroyer();
	return "";
}

void Ken::LeftRightCheck()
{
	if ("P1" == playerTag_)
	{
		Object* otherPlayer = GetScene()->FindObject("P2");
		if (GetPos().ix() > otherPlayer->GetPos().ix())
		{
			dirstate_ = ACTORDIR::LOOK_LEFT;
			bodyBox_->SetPivot({ -1.0f * bodyBoxPivot_.x, bodyBoxPivot_.y });
			//hitBoxHead_->SetPivot({ -1.0f * hitBoxHeadPivot_.x, hitBoxHeadPivot_.y });
			hitBoxTorso_->SetPivot({ -1.0f * hitBoxTorsoPivot_.x, hitBoxTorsoPivot_.y });
			//hitBoxLeg_->SetPivot({ -1.0f * hitBoxLegPivot_.x, hitBoxLegPivot_.y });
			strikeBox_->SetPivot({ -1.0f * strikeBoxPivot_.x, strikeBoxPivot_.y });
			guardRangeBox_->SetPivot({ -1.0f * guardRangeBoxPivot_.x, guardRangeBoxPivot_.y });
			guardBox_->SetPivot({ -1.0f * guardBoxPivot_.x, guardBoxPivot_.y });
			crouchGuardBox_->SetPivot({ -1.0f * crouchGuardBoxPivot_.x, crouchGuardBoxPivot_.y });
		}
		else if (GetPos().ix() < otherPlayer->GetPos().ix())
		{
			dirstate_ = ACTORDIR::LOOK_RIGHT;
			bodyBox_->SetPivot({ bodyBoxPivot_.x, bodyBoxPivot_.y });
			//hitBoxHead_->SetPivot({ hitBoxHeadPivot_.x, hitBoxHeadPivot_.y });
			hitBoxTorso_->SetPivot({ hitBoxTorsoPivot_.x, hitBoxTorsoPivot_.y });
			//hitBoxLeg_->SetPivot({ hitBoxLegPivot_.x, hitBoxLegPivot_.y });
			strikeBox_->SetPivot({ strikeBoxPivot_.x, strikeBoxPivot_.y });
			guardRangeBox_->SetPivot({ guardRangeBoxPivot_.x, guardRangeBoxPivot_.y });
			guardBox_->SetPivot({ guardBoxPivot_.x, guardBoxPivot_.y });
			crouchGuardBox_->SetPivot({ crouchGuardBoxPivot_.x, crouchGuardBoxPivot_.y });
		}
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
	}
	if ("P2" == playerTag_)
	{
		Object* otherPlayer = GetScene()->FindObject("P1");
		if (GetPos().ix() > otherPlayer->GetPos().ix())
		{
			dirstate_ = ACTORDIR::LOOK_LEFT;
			bodyBox_->SetPivot({ -1.0f * bodyBoxPivot_.x, bodyBoxPivot_.y });
			//hitBoxHead_->SetPivot({ -1.0f * hitBoxHeadPivot_.x, hitBoxHeadPivot_.y });
			hitBoxTorso_->SetPivot({ -1.0f * hitBoxTorsoPivot_.x, hitBoxTorsoPivot_.y });
			//hitBoxLeg_->SetPivot({ -1.0f * hitBoxLegPivot_.x, hitBoxLegPivot_.y });
			strikeBox_->SetPivot({ -1.0f * strikeBoxPivot_.x, strikeBoxPivot_.y });
			guardRangeBox_->SetPivot({ -1.0f * guardRangeBoxPivot_.x, guardRangeBoxPivot_.y });
			guardBox_->SetPivot({ -1.0f * guardBoxPivot_.x, guardBoxPivot_.y });
			crouchGuardBox_->SetPivot({ -1.0f * crouchGuardBoxPivot_.x, crouchGuardBoxPivot_.y });
		}
		else if (GetPos().ix() < otherPlayer->GetPos().ix())
		{
			dirstate_ = ACTORDIR::LOOK_RIGHT;
			bodyBox_->SetPivot({ bodyBoxPivot_.x, bodyBoxPivot_.y });
			//hitBoxHead_->SetPivot({ hitBoxHeadPivot_.x, hitBoxHeadPivot_.y });
			hitBoxTorso_->SetPivot({ hitBoxTorsoPivot_.x, hitBoxTorsoPivot_.y });
			//hitBoxLeg_->SetPivot({ hitBoxLegPivot_.x, hitBoxLegPivot_.y });
			strikeBox_->SetPivot({ strikeBoxPivot_.x, strikeBoxPivot_.y });
			guardRangeBox_->SetPivot({ guardRangeBoxPivot_.x, guardRangeBoxPivot_.y });
			guardBox_->SetPivot({ guardBoxPivot_.x, guardBoxPivot_.y });
			crouchGuardBox_->SetPivot({ crouchGuardBoxPivot_.x, crouchGuardBoxPivot_.y });
		}
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
	}

}

void Ken::Update()
{
	if (false == IsOn())
	{
		LeftRightCheck();
		return;
	}

	if (false == isDown_ && nullptr != collisionBox_->CollisionGroupCheckSingle(enemyCollision_)) // 상대방의 전진으로 밀려남의 구현입니다.
	{
		switch (dirstate_)
		{
		case ACTORDIR::LOOK_RIGHT:
		{
			curDirectionX_ = -1;
			if (85.0f <= GetCameraLinkedPos().x)
			{
				SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 150.0f);
			}
			break;
		}
		break;
		case ACTORDIR::LOOK_LEFT:
		{
			curDirectionX_ = 1;
			if (1067.0f >= GetCameraLinkedPos().x)
			{
				SetMove(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 150.0f);
			}
			break;
		}
		break;
		case ACTORDIR::MAX:
			break;
		default:
			break;
		}
	}

	if (true == IsOn())
	{
		switch (stateType_)
		{
		case KenState::Idle:
			Idle();
			break;
		case KenState::Walk:
			Walk();
			break;
		case KenState::Crouch:
			Crouch();
			break;
		case KenState::Jump:
			Jump();
			break;
		case KenState::JumpFront:
			JumpFront();
			break;
		case KenState::JumpBack:
			JumpBack();
			break;
		case KenState::Guard:
			Guard();
			break;
		case KenState::CrouchGuard:
			CrouchGuard();
			break;
		case KenState::HitUpperLight:
			HitUpperLight();
			break;
		case KenState::HitUpperHeavy:
			HitUpperHeavy();
			break;
		case KenState::HitDownLight:
			HitDownLight();
			break;
		case KenState::HitDownHeavy:
			HitDownHeavy();
			break;
		case KenState::HitWhenCrouch:
			HitWhenCrouch();
			break;
		case KenState::HitWhenJump:
			HitWhenJump();
			break;
		case KenState::HitWhenJumpDown:
			HitWhenJumpDown();
			break;
		case KenState::Down:
			Down();
			break;
		case KenState::Recovery:
			Recovery();
			break;
		case KenState::GetUp:
			GetUp();
			break;
		case KenState::KO:
			KO();
			break;
		case KenState::JumpKO:
			JumpKO();
			break;
		case KenState::R1Win:
			Win();
			break;
		case KenState::MatchPointWin:
			MatchPointWin();
			break;
		case KenState::StandDelay:
			StandDelay();
			break;
		case KenState::LandDelay:
			LandDelay();
			break;
		case KenState::LP:
			LP();
			break;
		case KenState::LP_Close:
			LP_Close();
			break;
		case KenState::LP_Crouch:
			LP_Crouch();
			break;
		case KenState::LP_Jump:
			LP_Jump();
			break;
		case KenState::LP_JumpFront:
			LP_JumpFront();
			break;
		case KenState::LP_JumpBack:
			LP_JumpBack();
			break;
		case KenState::MP:
			MP();
			break;
		case KenState::MP_Close:
			MP_Close();
			break;
		case KenState::MP_Crouch:
			MP_Crouch();
			break;
		case KenState::MP_Jump:
			MP_Jump();
			break;
		case KenState::MP_JumpFront:
			MP_JumpFront();
			break;
		case KenState::MP_JumpBack:
			MP_JumpBack();
			break;
		case KenState::HP:
			HP();
			break;
		case KenState::HP_Close:
			HP_Close();
			break;
		case KenState::HP_Crouch:
			HP_Crouch();
			break;
		case KenState::HP_Jump:
			HP_Jump();
			break;
		case KenState::HP_JumpFront:
			HP_JumpFront();
			break;
		case KenState::HP_JumpBack:
			HP_JumpBack();
			break;
		case KenState::LK:
			LK();
			break;
		case KenState::LK_Close:
			LK_Close();
			break;
		case KenState::LK_Crouch:
			LK_Crouch();
			break;
		case KenState::LK_Jump:
			LK_Jump();
			break;
		case KenState::LK_JumpFront:
			LK_JumpFront();
			break;
		case KenState::LK_JumpBack:
			LK_JumpBack();
			break;
		case KenState::MK:
			MK();
			break;
		case KenState::MK_Close:
			MK_Close();
			break;
		case KenState::MK_Crouch:
			MK_Crouch();
			break;
		case KenState::MK_Jump:
			MK_Jump();
			break;
		case KenState::MK_JumpFront:
			MK_JumpFront();
			break;
		case KenState::MK_JumpBack:
			MK_JumpBack();
			break;
		case KenState::HK:
			HK();
			break;
		case KenState::HK_Close:
			HK_Close();
			break;
		case KenState::HK_Crouch:
			HK_Crouch();
			break;
		case KenState::HK_Jump:
			HK_Jump();
			break;
		case KenState::HK_JumpFront:
			HK_JumpFront();
			break;
		case KenState::HK_JumpBack:
			HK_JumpBack();
			break;
		case KenState::Hadouken:
			Hadouken();
			break;
		case KenState::Shoryuken:
			Shoryuken();
			break;
		case KenState::Tatsumaki:
			Tatsumaki();
			break;
		case KenState::Throw:
			Throw();
			break;
		case KenState::BeThrown:
			BeThrown();
			break;
		case KenState::BeThrownDown:
			BeThrownDown();
			break;
		case KenState::Burn:
			Burn();
			break;
		case KenState::AIIdle:
			AIIdle();
			break;
		case KenState::AIWalk:
			AIWalk();
			break;
		case KenState::AICrouch:
			AICrouch();
			break;
		case KenState::AIJump:
			AIJump();
			break;
		case KenState::AIJumpFront:
			AIJumpFront();
			break;
		case KenState::AIJumpBack:
			AIJumpBack();
			break;
		default:
			break;
		}
	}
}

void Ken::Idle()
{
	soundPlayer_->PlayCountReset(1);
	voicePlayer_->PlayCountReset(1);
	ResetStrikeBox();
	ResetGuardBox();
	ResetHitBox();
	ResetBodyBox();
	ResetCollisionBox();
	SetPos({ GetPos().x, 450.0f });
	isDown_ = false;
	isAttacking_ = false;

	curDirectionX_ = 0;
	curDirectionY_ = 0;

	LeftRightCheck();

	if (true == isAIOn_)
	{
		return ChangeState(KenState::AIIdle);
	}

	if (true == isSessionOver_)
	{
		return;
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") || true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		return ChangeState(KenState::Walk);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(KenState::Crouch);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		return ChangeState(KenState::Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::LP_Close);
		}
		else
		{
			return ChangeState(KenState::LP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::MP_Close);
		}
		else
		{
			return ChangeState(KenState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::HP_Close);
		}
		else
		{
			return ChangeState(KenState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::LK_Close);
		}
		else
		{
			return ChangeState(KenState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::MK_Close);
		}
		else
		{
			return ChangeState(KenState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::HK_Close);
		}
		else
		{
			return ChangeState(KenState::HK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
	{
		return ChangeState(KenState::Hadouken);
	}
}

void Ken::Walk()
{
	LeftRightCheck();

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		if (1252.0f >= GetPos().x && 1067.0f >= GetCameraLinkedPos().x)
		{
			if (dirstate_ == ACTORDIR::LOOK_LEFT || nullptr == collisionBox_->CollisionGroupCheckSingle(enemyCollision_))
			{
				curDirectionX_ = 1;
				SetMove(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * moveSpeed_);
			}
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
	{
		if (-100.0f <= GetPos().x && 85.0f <= GetCameraLinkedPos().x)
		{
			if (dirstate_ == ACTORDIR::LOOK_RIGHT || nullptr == collisionBox_->CollisionGroupCheckSingle(enemyCollision_))
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * moveSpeed_);
			}
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(KenState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(KenState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(KenState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(KenState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP") && nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{ // 잡기 시동
			switch (dirstate_)
			{
			case CharacterHandle::ACTORDIR::LOOK_RIGHT:
				return ChangeState(KenState::Throw);
				break;
			case CharacterHandle::ACTORDIR::LOOK_LEFT:
				break;
			case CharacterHandle::ACTORDIR::MAX:
				break;
			default:
				break;
			}
		}
		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
		{
			switch (dirstate_)
			{
			case ACTORDIR::LOOK_RIGHT:
				return ChangeState(KenState::JumpFront);
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(KenState::JumpBack);
				break;
			default:
				break;
			}
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			switch (dirstate_)
			{
			case ACTORDIR::LOOK_RIGHT: // 오른 쪽을 바라보고 있으며 + 오른쪽 화살표키 + 필살기키를 누르면
				return ChangeState(KenState::Shoryuken); // 승룡권
				break;
			case ACTORDIR::LOOK_LEFT: // 왼 쪽을 바라보고 있으며 + 오른쪽 화살표키 + 필살기키를 누르면
				return ChangeState(KenState::Tatsumaki); // 용권선풍각
				break;
			default:
				break;
			}
		}


	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP") && nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			switch (dirstate_)
			{
			case CharacterHandle::ACTORDIR::LOOK_RIGHT:
				break;
			case CharacterHandle::ACTORDIR::LOOK_LEFT:
				return ChangeState(KenState::Throw);
				break;
			case CharacterHandle::ACTORDIR::MAX:
				break;
			default:
				break;
			}
		}

		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
		{
			switch (dirstate_)
			{
			case ACTORDIR::LOOK_RIGHT:
				return ChangeState(KenState::JumpBack);
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(KenState::JumpFront);
				break;
			default:
				break;
			}
		}




		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			switch (dirstate_)
			{
			case ACTORDIR::LOOK_RIGHT: // 오른 쪽을 바라보고 있으며, 왼쪽 화살표키 + 필살기키를 누르면
				return ChangeState(KenState::Tatsumaki); // 용권선풍각
				break;
			case ACTORDIR::LOOK_LEFT: // 왼 쪽을 바라보고 있으며 + 왼쪽 화살표키 + 필살기키를 누르면
				return ChangeState(KenState::Shoryuken); // 승룡권
				break;
			default:
				break;
			}
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(KenState::Crouch);
	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::LP_Close);
		}
		else
		{
			return ChangeState(KenState::LP);
		}

	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::MP_Close);
		}
		else
		{
			return ChangeState(KenState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::HP_Close);
		}
		else
		{
			return ChangeState(KenState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::LK_Close);
		}
		else
		{
			return ChangeState(KenState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::MK_Close);
		}
		else
		{
			return ChangeState(KenState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(KenState::HK_Close);
		}
		else
		{
			return ChangeState(KenState::HK);
		}
	}

	if (
		false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right")
		&& false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left")
		)
	{
		return ChangeState(KenState::Idle); // 꼭 return 으로 해주기.. 왜? if문 밖에 또 ChangeState() 있을 수 있으니...
	}
}

void Ken::Crouch()
{
	soundPlayer_->PlayCountReset(1);
	voicePlayer_->PlayCountReset(1);
	LeftRightCheck();

	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,60 }));
	hitBoxTorsoPivot_ = { 20, 60 };
	hitBoxTorso_->SetSize({ 100, 155 });

	ResetStrikeBox();
	ResetGuardBox();

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
			ACTORDIR::LOOK_RIGHT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
		{
			if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
				|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
			{
				return ChangeState(KenState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
			ACTORDIR::LOOK_RIGHT == dirstate_)
		{
			// 파동권이 존재한다는 것이니까.
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(KenState::CrouchGuard);
			}
		}

		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
		{
			if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
				|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
			{
				return ChangeState(KenState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_)
		{
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(KenState::CrouchGuard);
			}
		}

		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
		{
			return ChangeState(KenState::LP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
		{
			return ChangeState(KenState::MP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
		{
			return ChangeState(KenState::HP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
		{
			return ChangeState(KenState::LK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
		{
			return ChangeState(KenState::MK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
		{
			return ChangeState(KenState::HK_Crouch);
		}

		return ChangeState(KenState::Crouch);
	}

	if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::Jump()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(KenState::LP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(KenState::MP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(KenState::HP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(KenState::LK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(KenState::MK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(KenState::HK_Jump);
	}
	if (GetModifiedSpriteNumber(9) < mainSpriteRender_->GetCurAnimationFrame()) // 9번 애니메이션(3프레임) 의 지연시간 부여
	{

		if (0 >= JumpDir.y) // 0을 지나 마이너스로 가면 하강할 타이밍입니다.
		{
			curDirectionY_ = 1;
			float4 PrevPos = JumpDir;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		if (450 <= GetPos().iy()) 
		{
			SetPos({ GetPos().x, 450.0f }); 
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
	}
	return;
}

void Ken::JumpFront()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(KenState::LP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(KenState::MP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(KenState::HP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(KenState::LK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(KenState::MK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(KenState::HK_JumpFront);
	}
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (1092.0f > GetPos().x)
			{
				curDirectionX_ = 1;
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				isJump_ = false;
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{

			if (60.0f < GetPos().x)
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				isJump_ = false;
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::JumpBack()
{
	ResetGuardBox();

	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(KenState::LP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(KenState::MP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(KenState::HP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(KenState::LK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(KenState::MK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(KenState::HK_JumpBack);
	}
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;

	case ACTORDIR::LOOK_LEFT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (1092.0f > GetPos().x)
			{
				curDirectionX_ = 1;
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				return ChangeState(KenState::LandDelay);
				break;
			}
			return;
			break;
		}
	default:
		return;
		break;
	}
}

void Ken::LP()
{
	SetStrikeBox(mainSpriteRender_, 28, 29, 200, { 80,-45 }, { 128,30 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::LP_Close()
{
	SetStrikeBox(mainSpriteRender_, 36, 37, 200, { 80,-70 }, { 80,35 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}


void Ken::LP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 72, 73, 200, { 110, 15 }, { 110,35 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Crouch);
	}
}

void Ken::LP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 50, -100 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
	if (true == isJump_)
	{

		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}

		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		if (450.0f <= GetPos().iy())
		{
			return ChangeState(KenState::LandDelay);
		}
		return;
	}
}

void Ken::LP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 50, -100 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (60.0f < GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::LP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 50, -100 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (16 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				curDirectionY_ = 1;

				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			curDirectionY_ = -1;
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;

	case ACTORDIR::LOOK_LEFT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::MP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({20,15}) };
	hitBoxTorso_->SetSize({ 120, 210 });

	SetStrikeBox(mainSpriteRender_, 32, 33, 200, { 150, -65 }, { 105,65 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::MP_Close()
{
	SetStrikeBox(mainSpriteRender_, 40, 41, 200, { 100, -30 }, { 130,80 }, 22, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::MP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 75, 76, 200, { 120, 15 }, { 110,40 }, 22, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Crouch);
	}
}

void Ken::MP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 70, -80 }, { 120,50 }, 24, HITSTYLE::DOWNHEAVY, "MID");
	if (true == isJump_)
	{

		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		if (450.0f <= GetPos().iy())
		{
			return ChangeState(KenState::LandDelay);
		}
		return;
	}
}

void Ken::MP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 70, -80 }, { 120,50 }, 24, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (60.0f < GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::MP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 70, -80 }, { 120,50 }, 24, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (16 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Ken::HP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({30,15}) };
	hitBoxTorso_->SetSize({ 130, 210 });
	SetStrikeBox(mainSpriteRender_, 32, 33, 200, { 160, -65 }, { 115,75 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}


void Ken::HP_Close()
{
	SetStrikeBox(mainSpriteRender_, 45, 46, 200, { 120, -30 }, { 130,100 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	SetStrikeBox(mainSpriteRender_, 46, 47, 200, { 120, -120 }, { 130,100 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (GetModifiedSpriteNumber(46) == mainSpriteRender_->GetCurAnimationFrame() || GetModifiedSpriteNumber(47) == mainSpriteRender_->GetCurAnimationFrame())
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			SetInFightText();
			ChangeState(KenState::Shoryuken);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::HP_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorso_->SetSize({ 100,220 });
	SetStrikeBox(mainSpriteRender_, 79, 80, 200, { 100, -50 }, { 120,120 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	SetStrikeBox(mainSpriteRender_, 80, 81, 200, { 100, -90 }, { 90,170 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Crouch);
	}
}

void Ken::HP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	if (true == isJump_)
	{
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		if (450.0f <= GetPos().iy())
		{
			return ChangeState(KenState::LandDelay);
		}
		return;
	}
}

void Ken::HP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = 1;

			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (60.0f < GetPos().x)
		{
			curDirectionX_ = -1;

			SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::HP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 200, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (16 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;

				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Ken::LK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });
	SetStrikeBox(mainSpriteRender_, 51, 52, 200, { 40, -80 }, { 160,85 }, 14, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::LK_Close()
{
	SetStrikeBox(mainSpriteRender_, 59, 60, 200, { 120, 50 }, { 160,110 }, 14, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::LK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 84, 85, 200, { 110, 115 }, { 160,55 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Crouch);
	}
}

void Ken::LK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 200, { 20, -90 }, { 150,100 }, 12, HITSTYLE::DOWNHEAVY, "MID");
	if (true == isJump_)
	{
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;


		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		if (450.0f <= GetPos().iy())
		{
			return ChangeState(KenState::LandDelay);
		}
		return;
	}
}

void Ken::LK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 103, 104, 200, { 20, -20 }, { 100,75 }, 12, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = 1;

			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (60.0f < GetPos().x)
		{
			curDirectionX_ = -1;

			SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::LK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 103, 104, 200, { 20, -20 }, { 100,75 }, 12, HITSTYLE::DOWNHEAVY, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (16 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;

				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Ken::MK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });
	SetStrikeBox(mainSpriteRender_, 51, 52, 200, { 40, -80 }, { 170,95 }, 24, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::MK_Close()
{
	SetStrikeBox(mainSpriteRender_, 63, 64, 200, { 100, -10 }, { 100,100 }, 24, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::MK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 87, 88, 200, { 160, 115 }, { 200,55 }, 22, HITSTYLE::DOWNHEAVY, "LOW");

	if (GetModifiedSpriteNumber(87) <= mainSpriteRender_->GetCurAnimationFrame() && GetModifiedSpriteNumber(89) >= mainSpriteRender_->GetCurAnimationFrame())
	{
		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			return ChangeState(KenState::Hadouken);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Crouch);
	}
}

void Ken::MK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 200, { 20, -90 }, { 150,100 }, 22, HITSTYLE::DOWNHEAVY, "MID");
	if (true == isJump_)
	{

		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;


		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		if (450.0f <= GetPos().iy())
		{

			return ChangeState(KenState::LandDelay);
		}
		return;
	}
}

void Ken::MK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 200, { 90, 0 }, { 200,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = 1;

			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (60.0f < GetPos().x)
		{
			curDirectionX_ = -1;

			SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::MK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 200, { 90, 0 }, { 200,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (16 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;

				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::HK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -10,35 }));
	hitBoxTorso_->SetSize({ 170, 185 });
	SetStrikeBox(mainSpriteRender_, 54, 55, 200, { 120, -80 }, { 140,105 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::HK_Close()
{
	SetStrikeBox(mainSpriteRender_, 68, 70, 200, { 100, -90 }, { 120,190 }, 26, HITSTYLE::HITUPPERHEAVY, "HIGH");
	SetStrikeBox(mainSpriteRender_, 69, 70, 200, { 100, -40 }, { 170,125 }, 4, HITSTYLE::DOWNHEAVY, "MID");
	// 애니메이션은 한 번 재생되고, 그 "재생" 시간동안 여기 있어야 한다면...
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::HK_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,60 }));
	hitBoxTorso_->SetSize({ 120, 165 });
	SetStrikeBoxLow(mainSpriteRender_, 90, 91, 200, { 155, 115 }, { 110,55 }, 30, HITSTYLE::DOWN, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Crouch);
	}
}

void Ken::HK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 99, 100, 200, { 50, -50 }, { 140,100 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	SetStrikeBoxOverHead(mainSpriteRender_, 100, 101, 200, { 50, -30 }, { 200,120 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	if (true == isJump_)
	{
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;


		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		if (450.0f <= GetPos().iy())
		{
			return ChangeState(KenState::LandDelay);
		}
		return;
	}
}

void Ken::HK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 200, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (60.0f < GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::HK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 200, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (16 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;

				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (1092.0f > GetPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			isJump_ = false;
			return ChangeState(KenState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Ken::Hadouken()
{
	if (GetModifiedSpriteNumber(110) == mainSpriteRender_->GetCurAnimationFrame())
	{
		hitBoxTorso_->SetPivot(SetModifiedHitbox({ 45,15 }));
		hitBoxTorso_->SetSize({ 200,190 });

		// projectile_ 이 파동권일 것이다.

		if (nullptr == projectile_ && false == isShooting_)
		{
			voicePlayer_->PlayAlone("Ken_Hadouken.mp3", 0);
			projectile_ = GetScene()->CreateObject<Projectile>();
			projectile_->SetName(GetName() + "Projectile");
			projectile_->SetParentCharacter(this);
			projectile_->SetShotDir();
			projectile_->CollisionInit();
			isShooting_ = true;
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}


void Ken::Shoryuken()
{
	if (0 > JumpDir.y)
	{
		hitBoxTorso_->SetPivot(SetModifiedHitbox({ 0,1000 }));
		hitBoxTorso_->SetSize({ 0,0 });
	}
	else
	{
		hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,0 }));
		hitBoxTorso_->SetSize({ 100,240 });
	}
	

	SetStrikeBox(mainSpriteRender_, 141, 142, 200, { 90,-130 }, { 120,190 }, 38, HITSTYLE::Burn, "MID");
	SetStrikeBox(mainSpriteRender_, 142, 143, 200, { 90,-130 }, { 120,190 }, 38, HITSTYLE::Burn, "MID");
	SetStrikeBox(mainSpriteRender_, 143, 144, 200, { 90,-130 }, { 120,190 }, 38, HITSTYLE::Burn, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (GetModifiedSpriteNumber(141) < mainSpriteRender_->GetCurAnimationFrame())
		{
			if (0 >= JumpDir.y)
			{
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

				if (1000 >= GetPos().ix())
				{
					JumpDir.x = moveSpeed_ * 0.50f;
				}
				curDirectionX_ = 1;
				curDirectionY_ = 1;
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			if (1000 <= GetPos().ix())
			{
				JumpDir.x = 0.0f;
			}
			else
			{
				JumpDir.x = 50.0f;
			}

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (GetModifiedSpriteNumber(141) < mainSpriteRender_->GetCurAnimationFrame())
		{
			if (0 >= JumpDir.y)
			{
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

				if (80 <= GetPos().ix())
				{
					JumpDir.x = -moveSpeed_ * 0.50f;
				}
				curDirectionX_ = -1;
				curDirectionY_ = 1;
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			if (0 >= GetPos().ix())
			{
				JumpDir.x = 0.0f;
			}
			else
			{
				JumpDir.x = -50.0f;
			}

			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				return ChangeState(KenState::LandDelay);
			}
		}
		return;
		break;
	default:
		break;
	}
}
void Ken::Tatsumaki()
{ 
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 0,-100 }));
	hitBoxTorso_->SetSize({ 140,210 });
	SetStrikeBox(mainSpriteRender_, 117, 120, 200, { 90,-100 }, { 170,80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 121, 122, 200, { -90,-100 }, { 170, 80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 123, 124, 200, { 90,-100 }, { 170,80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 125, 126, 200, { -90,-100 }, { 170,80 }, 38, HITSTYLE::DOWN, "HIGH");

	if (GetModifiedSpriteNumber(128) > mainSpriteRender_->GetCurAnimationFrame())
	{
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (1000 >= GetPos().ix())
			{
				SetMove(float4::RIGHT * moveSpeed_ * 1.4f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (80 <= GetPos().ix())
			{
				SetMove(float4::LEFT * moveSpeed_ * 1.4f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::MAX:
			break;
		default:
			break;
		}

	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::LandDelay);
	}
}

void Ken::Throw()
{
	RemoveCollisionBox();
	otherCharacter_->SetHold(33);
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::BeThrown()
{
	RemoveCollisionBox();
	if (GetModifiedSpriteNumber(197) < mainSpriteRender_->GetCurAnimationFrame())
	{
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (1000.0f >= GetCameraLinkedPos().x)
			{
				SetMove(float4::RIGHT * knockBackDir_ * 3.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 <= knockBackDir_)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (152.0f <= GetCameraLinkedPos().x)
			{
				SetMove(float4::LEFT * knockBackDir_ * 3.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 <= knockBackDir_)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
			break;
		case CharacterHandle::ACTORDIR::MAX:
			break;
		default:
			break;
		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			hp_ -= 33;
			return ChangeState(KenState::BeThrownDown);
		}

	}
}


void Ken::BeThrownDown()
{
	isDown_ = true;
	RemoveHitBox();
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f <= GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::RIGHT * knockBackDir_ * 0.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f });
			isJump_ = false;
			return ChangeState(KenState::GetUp);
		}

		return;
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f >= GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::LEFT * knockBackDir_ * 0.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f }); // 오차가 나올 때를 대비해 강제로 바닥 y 값에 맞춰 줍니다.
			isJump_ = false;
			return ChangeState(KenState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Ken::SetHold(int _damage)
{
	ChangeState(KenState::BeThrown);
}

void Ken::SetStrikeBox(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
	float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation)
{
	curHitstyle_ = _hitLocation;
	isAttacking_ = true;
	// 인자 : 기준이 될 랜더러, 판정박스를 재생할 프레임 번호, 판정박스를 끌 프레임 번호, 스프라이트 좌/우 뒤집히는 기준 순번, 피봇, 사이즈
	if (_targetFrame == _renderer->GetCurAnimationFrame()) // (1P방향 기준) 재생시키고 싶은 프레임에 도달하면...
	{
		strikeBox_->SetPivot({ _pivot.x, _pivot.y });
		strikeBox_->SetSize({ _size.x, _size.y });
	}
	else if (_targetFrame + _spriteLRBorder == _renderer->GetCurAnimationFrame()) // (2P방향 기준) 재생시키고 싶은 프레임에 도달하면...
	{
		strikeBox_->SetPivot({ -_pivot.x, _pivot.y });
		strikeBox_->SetSize({ _size.x, _size.y });
	}

	if (nullptr != strikeBox_->CollisionGroupCheckSingle(enemyGuard_) || nullptr != strikeBox_->CollisionGroupCheckSingle(enemyCrouchGuard_))
	{
		soundPlayer_->PlayCountReset(1);
		soundPlayer_->PlayOverlap("Block.wav", 0);
		playerFightEffects_->On();
		playerFightEffects_->GetMainSpriteRender()->ChangeSpriteAnimation("GuardEffect");
		otherCharacter_->GetInstantDamage(1);
	}
	else if (nullptr != strikeBox_->CollisionGroupCheckSingle(enemyHit_))
	{
		if (19 > _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("LHit.wav", 0);
		}
		else if (29 > _strikeDamage || 19 >= _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("MHit.wav", 0);
		}
		else if (29 <= _strikeDamage && 35 >= _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("HHit.wav", 0);
		}
		else if (35 < _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("CriticalHit.wav", 0);
		}
		playerFightEffects_->On();
		playerFightEffects_->GetMainSpriteRender()->ChangeSpriteAnimation("HitEffect");
		otherCharacter_->SetDamaged(_strikeDamage, _hitStyle); // 인자 (피해량, 피격 모션)
	}

	if (_exitFrame == _renderer->GetCurAnimationFrame() || _exitFrame + _spriteLRBorder == _renderer->GetCurAnimationFrame())
	{	// 이후 끄고 싶은 프레임에 도달하면 판정박스를 비활성화
		strikeBoxPivot_ = { 0,0 };
		strikeBox_->SetPivot({ 0,0 });
		strikeBox_->SetSize({ 0,0 });
		isAttacking_ = false;
	}
}

void Ken::SetStrikeBoxOverHead(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
	float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation)
{
	curHitstyle_ = _hitLocation;
	isAttacking_ = true;
	// 인자 : 기준이 될 랜더러, 판정박스를 재생할 프레임 번호, 판정박스를 끌 프레임 번호, 스프라이트 좌/우 뒤집히는 기준 순번, 피봇, 사이즈
	if (_targetFrame == _renderer->GetCurAnimationFrame()) // (1P방향 기준) 재생시키고 싶은 프레임에 도달하면...
	{
		strikeBoxOverHead_->SetPivot({ _pivot.x, _pivot.y });
		strikeBoxOverHead_->SetSize({ _size.x, _size.y });
	}
	else if (_targetFrame + _spriteLRBorder == _renderer->GetCurAnimationFrame()) // (2P방향 기준) 재생시키고 싶은 프레임에 도달하면...
	{
		strikeBoxOverHead_->SetPivot({ -_pivot.x, _pivot.y });
		strikeBoxOverHead_->SetSize({ _size.x, _size.y });
	}

	if (nullptr != strikeBoxOverHead_->CollisionGroupCheckSingle(enemyGuard_))
	{
		soundPlayer_->PlayCountReset(1);
		soundPlayer_->PlayOverlap("Block.wav", 0);
		playerFightEffects_->On();
		playerFightEffects_->GetMainSpriteRender()->ChangeSpriteAnimation("GuardEffect");
		otherCharacter_->GetInstantDamage(1);
	}
	else if (nullptr != strikeBoxOverHead_->CollisionGroupCheckSingle(enemyHit_) || nullptr != strikeBoxOverHead_->CollisionGroupCheckSingle(enemyCrouchGuard_))
	{
		if (19 > _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("LHit.wav", 0);
		}
		else if (29 > _strikeDamage || 19 >= _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("MHit.wav", 0);
		}
		else if (29 <= _strikeDamage && 35 >= _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("HHit.wav", 0);
		}
		else if (35 < _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("CriticalHit.wav", 0);
		}
		playerFightEffects_->On();
		playerFightEffects_->GetMainSpriteRender()->ChangeSpriteAnimation("HitEffect");
		otherCharacter_->SetDamaged(_strikeDamage, _hitStyle); // 인자 (피해량, 피격 모션)
	}

	if (_exitFrame == _renderer->GetCurAnimationFrame() || _exitFrame + _spriteLRBorder == _renderer->GetCurAnimationFrame())
	{	// 이후 끄고 싶은 프레임에 도달하면 판정박스를 비활성화
		strikeBoxOverHeadPivot_ = { 0,0 };
		strikeBoxOverHead_->SetPivot({ 0,0 });
		strikeBoxOverHead_->SetSize({ 0,0 });
		isAttacking_ = false;
	}
}

// 하단기 전용 스트라이크박스
void Ken::SetStrikeBoxLow(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
	float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation)
{
	curHitstyle_ = _hitLocation;
	isAttacking_ = true;
	// 인자 : 기준이 될 랜더러, 판정박스를 재생할 프레임 번호, 판정박스를 끌 프레임 번호, 스프라이트 좌/우 뒤집히는 기준 순번, 피봇, 사이즈
	if (_targetFrame == _renderer->GetCurAnimationFrame()) // (1P방향 기준) 재생시키고 싶은 프레임에 도달하면...
	{
		strikeBoxLow_->SetPivot({ _pivot.x, _pivot.y });
		strikeBoxLow_->SetSize({ _size.x, _size.y });
	}
	else if (_targetFrame + _spriteLRBorder == _renderer->GetCurAnimationFrame()) // (2P방향 기준) 재생시키고 싶은 프레임에 도달하면...
	{
		strikeBoxLow_->SetPivot({ -_pivot.x, _pivot.y });
		strikeBoxLow_->SetSize({ _size.x, _size.y });
	}

	if (nullptr != strikeBoxLow_->CollisionGroupCheckSingle(enemyCrouchGuard_))
	{
		soundPlayer_->PlayOverlap("Block.wav", 0);
		playerFightEffects_->On();
		playerFightEffects_->GetMainSpriteRender()->ChangeSpriteAnimation("GuardEffect");
		otherCharacter_->GetInstantDamage(1);
	}
	else if (nullptr != strikeBoxLow_->CollisionGroupCheckSingle(enemyHit_))
	{
		if (19 > _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("LHit.wav", 0);
		}
		else if (29 > _strikeDamage || 19 >= _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("MHit.wav", 0);
		}
		else if (29 <= _strikeDamage && 35 >= _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("HHit.wav", 0);
		}
		else if (35 < _strikeDamage)
		{
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("CriticalHit.wav", 0);
		}
		playerFightEffects_->On();
		playerFightEffects_->GetMainSpriteRender()->ChangeSpriteAnimation("HitEffect");
		otherCharacter_->SetDamaged(_strikeDamage, _hitStyle); // 인자 (피해량, 피격 모션)
	}

	if (_exitFrame == _renderer->GetCurAnimationFrame() || _exitFrame + _spriteLRBorder == _renderer->GetCurAnimationFrame())
	{	// 이후 끄고 싶은 프레임에 도달하면 판정박스를 비활성화
		strikeBoxLowPivot_ = { 0,0 };
		strikeBoxLow_->SetPivot({ 0,0 });
		strikeBoxLow_->SetSize({ 0,0 });
		isAttacking_ = false;
	}
}

void Ken::ResetStrikeBox()
{
	strikeBoxPivot_ = { 0,0 };
	strikeBox_->SetPivot({ 0,0 });
	strikeBox_->SetSize({ 0,0 });
	strikeBoxOverHeadPivot_ = { 0,0 };
	strikeBoxOverHead_->SetPivot({ 0,0 });
	strikeBoxOverHead_->SetSize({ 0,0 });
	strikeBoxLowPivot_ = { 0,0 };
	strikeBoxLow_->SetPivot({ 0,0 });
	strikeBoxLow_->SetSize({ 0,0 });
}

void Ken::ResetGuardBox()
{
	guardBoxPivot_ = { 0, 2000 };
	guardBox_->SetPivot({ 0,2000 });
	guardBox_->SetSize({ 10,10 });
	crouchGuardBoxPivot_ = { 0, 2000 };
	crouchGuardBox_->SetPivot({ 0, 2000 });
	crouchGuardBox_->SetSize({ 10,10 });
}


void Ken::ResetHitBox()
{
    //hitBoxHeadPivot_ = { 12, -75 };
	hitBoxTorsoPivot_ = { -10, 15 };
	//hitBoxLegPivot_ = { -8, 84 };
	//hitBoxHead_->SetSize({ 42,39 });
	hitBoxTorso_->SetSize({ 100,210 });
	//hitBoxLeg_->SetSize({ 100,105 });
}

void Ken::ResetBodyBox()
{
	bodyBox_->SetPivot({ 0,15 });
	bodyBox_->SetSize({ 120,220 });
}


void Ken::SetDamaged(int _damage, HITSTYLE _hitStyle)
{
	hp_ -= _damage; // 체력 차감
	if (stateType_ == KenState::Crouch || stateType_ == KenState::CrouchGuard) // 피격자가 앉아 있을 때는 무조건 앉은 상태의 피격 모션이 사용된다.
	{
		return ChangeState(KenState::HitWhenCrouch);
	}

	if (stateType_ == KenState::Shoryuken || stateType_ == KenState::Tatsumaki)
	{
		return ChangeState(KenState::HitWhenJump);
	}

	if (stateType_ == KenState::Jump || stateType_ == KenState::JumpBack || stateType_ == KenState::JumpFront || stateType_ == KenState::LP_Jump || stateType_ == KenState::LP_JumpBack ||
		stateType_ == KenState::LP_JumpFront || stateType_ == KenState::MP_Jump || stateType_ == KenState::MP_JumpBack || stateType_ == KenState::MP_JumpFront || stateType_ == KenState::HP_Jump ||
		stateType_ == KenState::HP_JumpBack || stateType_ == KenState::HP_JumpFront || stateType_ == KenState::LK_Jump || stateType_ == KenState::LK_JumpBack || stateType_ == KenState::LK_JumpFront ||
		stateType_ == KenState::MK_Jump || stateType_ == KenState::MK_JumpBack || stateType_ == KenState::MK_JumpFront || stateType_ == KenState::HK_Jump || stateType_ == KenState::HK_JumpBack || stateType_ == KenState::HK_JumpFront)
	{
		if (HITSTYLE::Burn == _hitStyle)
		{
			return ChangeState(KenState::Burn);
		}
		return ChangeState(KenState::HitWhenJump);
	}
	// 대미지 크기와 유형에 따른 모션 변경?
	switch (_hitStyle)
	{
	case CharacterHandle::HITSTYLE::HITUPPERLIGHT:
		return ChangeState(KenState::HitUpperLight);
		break;
	case CharacterHandle::HITSTYLE::HITUPPERHEAVY:
		return ChangeState(KenState::HitUpperHeavy);
		break;
	case CharacterHandle::HITSTYLE::DOWNLIGHT:
		return ChangeState(KenState::HitDownLight);
		break;
	case CharacterHandle::HITSTYLE::DOWNHEAVY:
		return ChangeState(KenState::HitDownHeavy);
		break;
	case CharacterHandle::HITSTYLE::CROUCH:
		return ChangeState(KenState::HitWhenCrouch);
		break;
	case CharacterHandle::HITSTYLE::JUMP:
		return ChangeState(KenState::HitWhenJump);
		break;
	case CharacterHandle::HITSTYLE::DOWN:
		return ChangeState(KenState::Down);
		break;
	case CharacterHandle::HITSTYLE::BeThrownDown:
		return ChangeState(KenState::BeThrownDown);
		break;
	case CharacterHandle::HITSTYLE::Burn:
		return ChangeState(KenState::Burn);
		break;
	case CharacterHandle::HITSTYLE::MAX:
		break;
	default:
		break;
	}
}



void Ken::HitUpperHeavy()
{

	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;

			if (GetModifiedSpriteNumber(165) < mainSpriteRender_->GetCurAnimationFrame())
			{
				SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(165) < mainSpriteRender_->GetCurAnimationFrame())
			{
				otherCharacter_->SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			return ChangeState(KenState::Idle);
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(165) < mainSpriteRender_->GetCurAnimationFrame())
			{
				SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(165) < mainSpriteRender_->GetCurAnimationFrame())
			{
				otherCharacter_->SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{ 
			return ChangeState(KenState::Idle);
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void Ken::HitUpperLight()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			if (GetModifiedSpriteNumber(165) <= mainSpriteRender_->GetCurAnimationFrame())
			{
				SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(165) < mainSpriteRender_->GetCurAnimationFrame())
			{
				otherCharacter_->SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(165) <= mainSpriteRender_->GetCurAnimationFrame())
			{
				SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(165) < mainSpriteRender_->GetCurAnimationFrame())
			{
				otherCharacter_->SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ken::HitDownHeavy()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			if (GetModifiedSpriteNumber(161) <= mainSpriteRender_->GetCurAnimationFrame())
			{
				SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(161) < mainSpriteRender_->GetCurAnimationFrame())
			{
				otherCharacter_->SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}


		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(161) <= mainSpriteRender_->GetCurAnimationFrame())
			{
				SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(161) < mainSpriteRender_->GetCurAnimationFrame())
			{
				otherCharacter_->SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ken::HitDownLight()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
				otherCharacter_->SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
				otherCharacter_->SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ken::HitWhenCrouch()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
			otherCharacter_->SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
			otherCharacter_->SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			ChangeState(KenState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Ken::HitWhenJump()
{
	RemoveHitBox();
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f <= GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(KenState::HitWhenJumpDown);
		}

		return;
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f >= GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(KenState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ken::HitWhenJumpDown()
{
	RemoveHitBox();
	RemoveBodyBox();

	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f <= GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * knockBackDir_ * 0.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}

		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
		{
			SetPos({ GetPos().x, 450.0f });
			isJump_ = false;
			return ChangeState(KenState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f });
			isJump_ = false;
			return ChangeState(KenState::GetUp);
		}

		return;
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f >= GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * knockBackDir_ * 0.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}


		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
		{
			SetPos({ GetPos().x, 450.0f });
			isJump_ = false;
			return ChangeState(KenState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f }); // 오차가 나올 때를 대비해 강제로 바닥 y 값에 맞춰 줍니다.
			isJump_ = false;
			return ChangeState(KenState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ken::Down()
{
	RemoveHitBox();
	RemoveBodyBox();
	ResetGuardBox();

	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f <= GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}


		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd() || 450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetPos({ GetPos().x, 450.0f });
			return ChangeState(KenState::GetUp);
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f >= GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}

		}
		if (0 >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd() || 450.0f <= GetPos().iy())
		{
			SetDustParticle();
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetPos({ GetPos().x, 450.0f });
			ChangeState(KenState::GetUp);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Ken::Burn()
{
	RemoveHitBox();
	RemoveBodyBox();

	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f <= GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(KenState::HitWhenJumpDown);
		}

		return;
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f >= GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(KenState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Ken::GetUp()
{
	RemoveHitBox();
	RemoveBodyBox();

	if (GetModifiedSpriteNumber(177) == mainSpriteRender_->GetCurAnimationFrame())
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			SetReversalText();
			return ChangeState(KenState::Shoryuken);
		}
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::StandDelay);
	}
}


void Ken::Recovery()
{
	RemoveHitBox();
	RemoveBodyBox();

	switch (dirstate_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
	{
		if (85.0f <= GetCameraLinkedPos().x)
		{
			SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (knockBackDir_ >= 0.0f)
		{
			knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
		}
	}
	break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
	{

		if (1067.0f >= GetCameraLinkedPos().x)
		{
			SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
		}
		if (knockBackDir_ >= 0.0f)
		{
			knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
		}
	}
	break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::KO()
{
	RemoveHitBox();
	RemoveBodyBox();

	if (true == IsOn())
	{
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				return;
			}
			if (85.0f <= GetCameraLinkedPos().x && true != mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * knockBackDir_ * 0.4f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 80.0f;
				if (knockBackDir_ <= 1.0f)
				{
					Off();
					SetMove({ 0,0 });
				}
				return;
			}
			curDirectionX_ = 0;
			return;
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				return;
			}
			if (1067.0f >= GetCameraLinkedPos().x && true != mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				curDirectionX_ = 1;
				SetMove(float4::RIGHT * knockBackDir_ * 0.4f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 80.0f;
				if (knockBackDir_ <= 1.0f)
				{
					Off();
					SetMove({ 0,0 });
				}
				return;
			}
			curDirectionX_ = 0;
			return;
			break;
		case CharacterHandle::ACTORDIR::MAX:
			break;
		default:
			break;
		}
	}

}

void Ken::JumpKO()
{
	RemoveHitBox();
	RemoveBodyBox();

	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f <= GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			return ChangeState(KenState::KO);
		}

		return;
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f >= GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			if (knockBackDir_ >= 0.0f)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
			}
		}
		if (0.0f >= JumpDir.y)
		{
			curDirectionY_ = 1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();


			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			return ChangeState(KenState::KO);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void Ken::Win()
{
	if ("P1" == GetName() && 1 <= FightScene::p1WinPoint_)
	{
		ChangeState(KenState::MatchPointWin);
	}
	if ("P2" == GetName() && 1 <= FightScene::p2WinPoint_)
	{
		ChangeState(KenState::MatchPointWin);
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}

void Ken::MatchPointWin()
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}

void Ken::StandDelay() // 4프레임의 기상 디스어드밴티지 -> 이 행동 동안은 가드밖에 할 수 없다.
{
	RemoveBodyBox();
	RemoveHitBox();
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(KenState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{
		// 파동권이 존재한다는 것이니까.
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(KenState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(KenState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(KenState::Guard);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::LandDelay() // 점프 후 착지 시 딜레이
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(KenState::Idle);
	}
}

void Ken::Guard()
{
	ResetHitBox();
	ResetGuardBox();
	guardBox_->SetPivot(SetModifiedHitbox({ 20,0 }));
	guardBoxPivot_ = { 20,0 };
	guardBox_->SetSize({ 100,200 });
	LeftRightCheck();

	switch (dirstate_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeOverHead_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeOverHead_))
			{
				otherCharacter_->SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeOverHead_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeOverHead_))
			{
				otherCharacter_->SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		ResetGuardBox();
		return ChangeState(KenState::CrouchGuard);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		ResetGuardBox();
		return ChangeState(KenState::JumpBack);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Right"))
	{
		return ChangeState(KenState::Idle);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Left"))
	{
		return ChangeState(KenState::Idle);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR")
		/* && false == otherCharacter_->GetProjectile()->IsOn()*/)
	{
		ResetGuardBox();
		return ChangeState(KenState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL")
		/*  && false == otherCharacter_->GetProjectile()->IsOn() */)
	{
		ResetGuardBox();
		return ChangeState(KenState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
	{
		ResetGuardBox();
		return ChangeState(KenState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		ResetGuardBox();
		return ChangeState(KenState::Walk);
	}
}

void Ken::CrouchGuard()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,60 }));
	hitBoxTorsoPivot_ = { 20, 60 };
	hitBoxTorso_->SetSize({ 100, 155 });
	ResetGuardBox();
	crouchGuardBox_->SetPivot(SetModifiedHitbox({ 30,0 }));
	crouchGuardBoxPivot_ = { 30,50 };
	crouchGuardBox_->SetSize({ 100,150 });
	LeftRightCheck();

	switch (dirstate_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeLow_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeLow_))
			{
				otherCharacter_->SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeLow_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
			if (nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardBox_->CollisionGroupCheckSingle(enemyStrikeLow_))
			{
				otherCharacter_->SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

	if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{

		return ChangeState(KenState::Guard);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR"))
	{
		return ChangeState(KenState::Crouch);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL"))
	{
		return ChangeState(KenState::Crouch);
	}
}

void Ken::UpdateAfter()
{
	if (0 >= otherCharacter_->GetCurHp())
	{
		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			if (true == mainSpriteRender_->IsCurSpriteAnimationName("Idle" + GetDirString()))
			{
				return ChangeState(KenState::R1Win);
			}
			else if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				return ChangeState(KenState::R1Win);
			}

		}
	}

}


void Ken::Collision()
{


}



// *** AI

void Ken::AIIdle()
{
	soundPlayer_->PlayCountReset(1);
	voicePlayer_->PlayCountReset(1);
	ResetStrikeBox();
	ResetGuardBox();
	ResetHitBox();
	ResetCollisionBox();

	SetPos({ GetPos().x, 450.0f });
	isDown_ = false;
	isAttacking_ = false;

	curDirectionX_ = 0;
	curDirectionY_ = 0;

	LeftRightCheck();

	if (false == isAIOn_)
	{
		return ChangeState(KenState::Idle);
	}

	if (true == isSessionOver_)
	{
		return;
	}

	// Idle 일 시 인공지능의 행동 요령
	// 가드한다 / 니가와 / 접근한다 / 후퇴한다
	if (nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_) || nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeOverHead_) || nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			diceRoll_ = randomGenerator_.RandomInt(0, 2);
			if (1 == diceRoll_)
			{
				return ChangeState(KenState::Guard);
			}
		}
	}
	else if (nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeLow_) || nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			diceRoll_ = randomGenerator_.RandomInt(0, 2);
			if (1 == diceRoll_)
			{
				return ChangeState(KenState::CrouchGuard);
			}
		}
	}

	if (nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		diceRoll_ = randomGenerator_.RandomInt(0, 2);
		if (diceRoll_ >= 1)
		{
			return ChangeState(KenState::AIJumpFront);
		}
	}

	diceRoll_ = randomGenerator_.RandomInt(0, 5);

	// 니가와
	if (diceRoll_ == 0 || diceRoll_ == 3)
	{
		return;
	}

	if (diceRoll_ == 1 || diceRoll_ == 2)
	{
		if (150.0f >= otherCharacter_->GetPos().x - GetPos().x || 150.0f >= GetPos().x - otherCharacter_->GetPos().x)
		{
			if (otherCharacter_->GetIsJump() == true)
			{
				return ChangeState(KenState::Shoryuken);
			}
		}
		if (300.0f <= otherCharacter_->GetPos().x - GetPos().x || 300.0f <= GetPos().x - otherCharacter_->GetPos().x)
		{
			return ChangeState(KenState::Hadouken);
		}
		diceRoll_ = randomGenerator_.RandomInt(0, 2);
		if (0 == diceRoll_)
		{
			return ChangeState(KenState::MK_Crouch);
		}
		if (1 == diceRoll_)
		{
			return ChangeState(KenState::HK);
		}
		if (2 == diceRoll_)
		{
			return ChangeState(KenState::AIJumpFront);
		}

	}

	if (diceRoll_ == 4 || diceRoll_ == 5)
	{
		return ChangeState(KenState::AIWalk);
	}
}

void Ken::AIWalk()
{
	LeftRightCheck();
	switch (diceRoll_)
	{
	case 0:
	{
		return ChangeState(KenState::AIIdle);
	}
	break;

	case 1:
	{
		return ChangeState(KenState::AIIdle);
	}
	break;

	case 2:
	{
		return ChangeState(KenState::AIIdle);
	}
	break;

	case 3:
	{
		return ChangeState(KenState::AIIdle);
	}
	break;

	case 4: // 상대에게 전진
	{
		if (dirstate_ == ACTORDIR::LOOK_LEFT && otherCharacter_->GetPos().x + 100.0f < GetPos().x)
		{
			if (85.0f < GetCameraLinkedPos().x)
			{
				return SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			return;
		}
		else if (dirstate_ == ACTORDIR::LOOK_LEFT && otherCharacter_->GetPos().x + 100.0f >= GetPos().x)
		{
			diceRoll_ = randomGenerator_.RandomInt(0, 3);
			switch (diceRoll_)
			{
			case 0:
			{
				return ChangeState(KenState::MK_Crouch);
			}
			break;
			case 1:
			{
				return ChangeState(KenState::AIJumpFront);
			}
			break;
			case 2:
			{
				return ChangeState(KenState::HK);
			}
			break;
			case 3:
			{
				return ChangeState(KenState::MK_Crouch);
			}
			break;
			default:
				break;
			}

		}
		else
		{
			return ChangeState(KenState::AIIdle);
		}

		if (dirstate_ == ACTORDIR::LOOK_RIGHT && otherCharacter_->GetPos().x - 100.0f > GetPos().x)
		{
			if (1067.0f > GetCameraLinkedPos().x)
			{
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}

		}
		else if (dirstate_ == ACTORDIR::LOOK_RIGHT && otherCharacter_->GetPos().x - 100.0f <= GetPos().x)
		{
			diceRoll_ = randomGenerator_.RandomInt(0, 3);
			switch (diceRoll_)
			{
			case 0:
			{
				return ChangeState(KenState::MK_Crouch);
			}
			break;
			case 1:
			{
				return ChangeState(KenState::AIJumpFront);
			}
			break;
			case 2:
			{
				return ChangeState(KenState::HK);
			}
			break;
			case 3:
			{
				return ChangeState(KenState::MK_Crouch);
			}
			break;
			default:
				break;
			}
		}
		else
		{
			return ChangeState(KenState::AIIdle);
		}
	}
	break;

	case 5: // 상대에게서 후퇴
	{
		if (dirstate_ == ACTORDIR::LOOK_LEFT && otherCharacter_->GetPos().x + 100.0f < GetPos().x)
		{
			if (1067.0f > GetCameraLinkedPos().x)
			{
				int roll = randomGenerator_.RandomInt(0, 10);
				if (10 == roll)
				{
					return ChangeState(KenState::AIJumpBack);
				}
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}

			if (GetPos().x - otherCharacter_->GetPos().x >= 400.0f)
			{
				return ChangeState(KenState::AIIdle);
			}
			return;
		}


		if (dirstate_ == ACTORDIR::LOOK_RIGHT && otherCharacter_->GetPos().x - 100.0f > GetPos().x)
		{
			if (85.0f < GetCameraLinkedPos().x)
			{
				int roll = randomGenerator_.RandomInt(0, 10);
				if (10 == roll)
				{
					return ChangeState(KenState::AIJumpBack);
				}
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}

			if (GetPos().x - otherCharacter_->GetPos().x <= -400.0f)
			{
				return ChangeState(KenState::AIIdle);
			}
			return;
		}
	}
	break;
	default:
		break;
	}
}

void Ken::AICrouch()
{

}

void Ken::AIJump()
{

}

void Ken::AIJumpFront()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (1092.0f > GetPos().x)
			{
				curDirectionX_ = 1;
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 >= JumpDir.y)
			{


				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;

			diceRoll_ = randomGenerator_.RandomInt(0, 3);
			switch (diceRoll_)
			{
			case 0:
				return ChangeState(KenState::LK_JumpFront);
				break;
			case 1:
				return ChangeState(KenState::MK_JumpFront);
				break;
			case 2:
				return ChangeState(KenState::HP_JumpFront);
				break;
			case 3:
				return ChangeState(KenState::MP_JumpFront);
				break;
			default:
				break;
			}

			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				isJump_ = false;
				return ChangeState(KenState::AIIdle);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{

			if (60.0f < GetPos().x)
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 >= JumpDir.y)
			{
				return ChangeState(KenState::LK_JumpFront);

				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				isJump_ = false;
				return ChangeState(KenState::AIIdle);
			}
		}
		return;
		break;
	default:
		break;
	}
}

void Ken::AIJumpBack()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });

	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (60.0f <= GetPos().ix())
			{
				curDirectionX_ = -1;
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0.0f >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				return ChangeState(KenState::AIIdle);
			}
		}
		return;
		break;

	case ACTORDIR::LOOK_LEFT:
		if (GetModifiedSpriteNumber(16) < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
		{
			if (1092.0f > GetPos().x)
			{
				curDirectionX_ = 1;
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 >= JumpDir.y)
			{
				curDirectionY_ = 1;
				JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				return;
			}
			curDirectionY_ = -1;
			JumpDir += float4::DOWN * 3000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

			if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
			{
				SetPos({ GetPos().x, 450.0f });
				return ChangeState(KenState::Idle);
				break;
			}
			return;
			break;
		}
	default:
		return;
		break;
	}
}