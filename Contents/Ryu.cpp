
#include "Ryu.h"
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
#include <EngineGeneralMath.h>
#include "ContentsENUM.h"
#include <GameCollision.h>
#include "FightScene.h"
#include "FightEffects.h"
#include "DustParticle.h"

Ryu::Ryu()
	: stateType_(RyuState::MAX), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr)
{
	hp_ = 180;
	originalHp_ = 180;
	maxJumpHeight_ = 220.0f;
	moveSpeed_ = 300.0f;
	SetRenderOrder(10);
	jumpForce_ = 1300.0f;
}

Ryu::~Ryu()
{

}

Ryu::Ryu(Ryu&& _other) noexcept
	: stateType_(_other.stateType_), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr)
{
}

void Ryu::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();

	if (true == isDebugOn_)
	{
		// *** 하기는 판정박스 디버그용 랜더링입니다.
		//bodyBox_->StartDebugRender();
		//collisionBox_->StartDebugRender();
		//hitBoxHead_->StartDebugRender();
		hitBoxTorso_->StartDebugRender();
		//hitBoxLeg_->StartDebugRender();
		//guardRangeBox_->StartDebugRender();
		//guardBox_->StartDebugRender();
		//crouchGuardBox_->StartDebugRender();
		strikeBox_->StartDebugRender();
		strikeBoxOverHead_->StartDebugRender();
		strikeBoxLow_->StartDebugRender();
		// ->StartDebugRender();
	}
}

void Ryu::ToggleDebug()
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

void Ryu::ToggleAI()
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

void Ryu::KeyInit()
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

		hitBoxTorso_ = CreateCollision(ST2GROUP::PLAYER1HIT, ColliderType::RECT);

		strikeBox_ = CreateCollision(ST2GROUP::PLAYER1STRIKE, ColliderType::RECT);
		// 인자 1 : 내 충돌체의 특징 이넘
		// 인자 2 : EngineENUM.h 에 들어있는 콜리전의 타입(박스, 포인트 등)

		//if (nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
		//{
		//
		//}

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
	// 초기 판정 박스들의 피벗과 크기 설정입니다.
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
	// 타격 박스는 크기와 피벗 없이 생성되며, 공격 애니메이션 재생 때 프레임마다 결정됩니다.

}

void Ryu::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("SpritesRyu.bmp");

	mainSpriteRender_->CreateSpriteAnimation("IdleL", "SpritesRyu.bmp", 0, 3, true, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("IdleR", "SpritesRyu.bmp", 0 + 210, 3 + 210, true, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("WalkL", "SpritesRyu.bmp", 4, 8, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("WalkR", "SpritesRyu.bmp", 4 + 210, 8 + 210, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("CrouchL", "SpritesRyu.bmp", 24, 24, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchR", "SpritesRyu.bmp", 24 + 210, 24 + 210, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("GuardL", "SpritesRyu.bmp", 25, 25, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardL", "SpritesRyu.bmp", 26, 26, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("GuardR", "SpritesRyu.bmp", 25 + 210, 25 + 210, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardR", "SpritesRyu.bmp", 26 + 210, 26 + 210, true, 0.1f);

	// 수직점프 애니메이션 설정
	mainSpriteRender_->CreateSpriteAnimation("JumpL", "SpritesRyu.bmp", 9, 14, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("JumpR", "SpritesRyu.bmp", 9 + 210, 14 + 210, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("JumpL", 9, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpL", 14, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpR", 9 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpR", 14 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("JumpFrontL", "SpritesRyu.bmp", 16, 22, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpFrontR", "SpritesRyu.bmp", 16 + 210, 22 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 16, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 17, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 21, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 22, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 16 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 17 + 210, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 21 + 210, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 22 + 210, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("JumpBackL", "SpritesRyu.bmp", 16, 22, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpBackR", "SpritesRyu.bmp", 16 + 210, 22 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 16, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 17, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 21, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 22, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 16 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 17 + 210, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 21 + 210, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 22 + 210, 0.204f);

	// 피격 & 다운 & 리커버리 & K.O. & 승리포즈
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightL", "SpritesRyu.bmp", 168, 169, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightR", "SpritesRyu.bmp", 168 + 210, 169 + 210, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyL", "SpritesRyu.bmp", 168, 172, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyR", "SpritesRyu.bmp", 168 + 210, 172 + 210, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownLightL", "SpritesRyu.bmp", 165, 165, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownLightR", "SpritesRyu.bmp", 165 + 210, 165 + 210, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyL", "SpritesRyu.bmp", 165, 168, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyR", "SpritesRyu.bmp", 165 + 210, 168 + 210, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchL", "SpritesRyu.bmp", 173, 173, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchR", "SpritesRyu.bmp", 173 + 210, 173 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpL", "SpritesRyu.bmp", 174, 175, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpR", "SpritesRyu.bmp", 174 + 210, 175 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownL", "SpritesRyu.bmp", 176, 177, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownR", "SpritesRyu.bmp", 176 + 210, 177 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownL", "SpritesRyu.bmp", 176, 177, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownR", "SpritesRyu.bmp", 176 + 210, 177 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownL", "SpritesRyu.bmp", 204, 207, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownR", "SpritesRyu.bmp", 204 + 210, 207 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("DownL", "SpritesRyu.bmp", 174, 177, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("DownR", "SpritesRyu.bmp", 174 + 210, 177 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BurnL", "SpritesRyu.bmp", 161, 163, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("BurnR", "SpritesRyu.bmp", 161 + 210, 163 + 210, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("RecoveryL", "SpritesRyu.bmp", 178, 181, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("RecoveryR", "SpritesRyu.bmp", 178 + 210, 181 + 210, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("GetUpL", "SpritesRyu.bmp", 177, 181, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("GetUpR", "SpritesRyu.bmp", 177 + 210, 181 + 210, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 177, 0.252f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 177 + 210, 0.252f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 178, 0.152f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 178 + 210, 0.152f);

	mainSpriteRender_->CreateSpriteAnimation("KOL", "SpritesRyu.bmp", 184, 189, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("KOR", "SpritesRyu.bmp", 184 + 210, 189 + 210, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("KOL", 189, 0.85f);
	mainSpriteRender_->SetChangeFrame("KOR", 189 + 210, 0.85f);

	mainSpriteRender_->CreateSpriteAnimation("R1WinL", "SpritesRyu.bmp", 193, 195, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("R1WinR", "SpritesRyu.bmp", 193 + 210, 195 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinL", "SpritesRyu.bmp", 197, 202, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinR", "SpritesRyu.bmp", 197 + 210, 202 + 210, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("StandDelayL", "SpritesRyu.bmp", 9,9,false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("StandDelayR", "SpritesRyu.bmp", 9 + 210,9 + 210,false, 0.085f); // 5프레임 지연

	mainSpriteRender_->CreateSpriteAnimation("LandDelayL", "SpritesRyu.bmp", 9, 9, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LandDelayR", "SpritesRyu.bmp", 9 + 210, 9 + 210, false, 0.034f); // 2프레임 지연

	// 기본기
	mainSpriteRender_->CreateSpriteAnimation("LPL", "SpritesRyu.bmp", 27, 29, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("LPR", "SpritesRyu.bmp", 27 + 210, 29 + 210, false, 0.05f);
	mainSpriteRender_->SetChangeFrame("LPL", 27, 0.051f);
	mainSpriteRender_->SetChangeFrame("LPR", 27 + 210, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("LP_CloseL", "SpritesRyu.bmp", 35, 37, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CloseR", "SpritesRyu.bmp", 35 + 210, 37 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 35, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 37, 0.085f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 35 + 210, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 37 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LP_JumpL", "SpritesRyu.bmp", 94, 95, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_JumpR", "SpritesRyu.bmp", 94 + 210, 95 + 210, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalL", "SpritesRyu.bmp", 94, 95, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalR", "SpritesRyu.bmp", 94 + 210, 95 + 210, false, 0.034f);


	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchL", "SpritesRyu.bmp", 71, 73, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchR", "SpritesRyu.bmp", 71 + 210, 73 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchL", 72, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchL", 73, 0.085f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchR", 72 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchR", 73 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("MPL", "SpritesRyu.bmp", 31, 33, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MPR", "SpritesRyu.bmp", 31 + 210, 33 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MPL", 33, 0.102f);
	mainSpriteRender_->SetChangeFrame("MPR", 33 + 210, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("MP_JumpL", "SpritesRyu.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_JumpR", "SpritesRyu.bmp", 94 + 210, 96 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_JumpL", 95, 0.3f);
	mainSpriteRender_->SetChangeFrame("MP_JumpR", 95 + 210, 0.3f);
	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalL", "SpritesRyu.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalR", "SpritesRyu.bmp", 94 + 210, 96 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_DiagonalL", 95, 0.3f);
	mainSpriteRender_->SetChangeFrame("MP_DiagonalR", 95 + 210, 0.3f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CloseL", "SpritesRyu.bmp", 39, 41, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CloseR", "SpritesRyu.bmp", 39 + 210, 41 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CloseL", 40, 0.153f);
	mainSpriteRender_->SetChangeFrame("MP_CloseL", 41, 0.170f);
	mainSpriteRender_->SetChangeFrame("MP_CloseR", 40 + 210, 0.153f);
	mainSpriteRender_->SetChangeFrame("MP_CloseR", 41 + 210, 0.170f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchL", "SpritesRyu.bmp", 74, 76, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchR", "SpritesRyu.bmp", 74 + 210, 76 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 75, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 76, 0.085f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 75 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 76 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HPL", "SpritesRyu.bmp", 31, 33, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HPR", "SpritesRyu.bmp", 31 + 210, 33 + 210, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("HPL", 33, 0.374f);
	mainSpriteRender_->SetChangeFrame("HPR", 33 + 210, 0.374f);

	mainSpriteRender_->CreateSpriteAnimation("HP_JumpL", "SpritesRyu.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_JumpR", "SpritesRyu.bmp", 94 + 210, 96 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_JumpL", 95, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_JumpR", 95 + 210, 0.34f);
	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalL", "SpritesRyu.bmp", 94, 96, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalR", "SpritesRyu.bmp", 94 + 210, 96 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalL", 95, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalR", 95 + 210, 0.34f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CloseL", "SpritesRyu.bmp", 45, 48, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CloseR", "SpritesRyu.bmp", 45 + 210, 48 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 46, 0.034f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 47, 0.102f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 48, 0.391f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 46 + 210, 0.034f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 47 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 48 + 210, 0.391f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchL", "SpritesRyu.bmp", 78, 82, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchR", "SpritesRyu.bmp", 78 + 210, 82 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 79, 0.051f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 80, 0.136f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 81, 0.204f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 82, 0.187f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 79 + 210, 0.051f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 80 + 210, 0.136f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 81 + 210, 0.204f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 82 + 210, 0.187f);

	mainSpriteRender_->CreateSpriteAnimation("LKL", "SpritesRyu.bmp", 50, 52, false, 0.119f);
	mainSpriteRender_->CreateSpriteAnimation("LKR", "SpritesRyu.bmp", 50 + 210, 52 + 210, false, 0.119f);
	mainSpriteRender_->SetChangeFrame("LKL", 51, 0.136f);
	mainSpriteRender_->SetChangeFrame("LKL", 52, 0.085f);
	mainSpriteRender_->SetChangeFrame("LKR", 51 + 210, 0.136f);
	mainSpriteRender_->SetChangeFrame("LKR", 52 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_JumpL", "SpritesRyu.bmp", 96, 98, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpR", "SpritesRyu.bmp", 96 + 210, 98 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_JumpL", 97, 0.68f);
	mainSpriteRender_->SetChangeFrame("LK_JumpR", 97 + 210, 0.68f);

	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalL", "SpritesRyu.bmp", 103, 103, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalR", "SpritesRyu.bmp", 103 + 210, 103 + 210, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_JumpFrontL", "SpritesRyu.bmp", 102, 103, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpFrontR", "SpritesRyu.bmp", 102 + 210, 103 + 210, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpBackL", "SpritesRyu.bmp", 102, 103, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpBackR", "SpritesRyu.bmp", 102 + 210, 103 + 210, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CloseL", "SpritesRyu.bmp", 58, 60, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CloseR", "SpritesRyu.bmp", 58 + 210, 60 + 210, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("LK_CloseL", 60, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CloseR", 60 + 210, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchL", "SpritesRyu.bmp", 83, 85, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchR", "SpritesRyu.bmp", 83 + 210, 85 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 84, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 85, 0.085f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 84 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 85 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("MKL", "SpritesRyu.bmp", 50, 52, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("MKR", "SpritesRyu.bmp", 50 + 210, 52 + 210, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("MKL", 52, 0.119f);
	mainSpriteRender_->SetChangeFrame("MKR", 52 + 210, 0.119f);

	mainSpriteRender_->CreateSpriteAnimation("MK_JumpL", "SpritesRyu.bmp", 96, 98, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_JumpR", "SpritesRyu.bmp", 96 + 210, 98 + 210, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_JumpL", 97, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_JumpR", 97 + 210, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalL", "SpritesRyu.bmp", 104, 106, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalR", "SpritesRyu.bmp", 104 + 210, 106 + 210, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalL", 105, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalR", 105 + 210, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CloseL", "SpritesRyu.bmp", 62, 64, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CloseR", "SpritesRyu.bmp", 62 + 210, 64 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CloseL", 63, 0.102f);
	mainSpriteRender_->SetChangeFrame("MK_CloseL", 64, 0.136f);
	mainSpriteRender_->SetChangeFrame("MK_CloseR", 63 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("MK_CloseR", 64 + 210, 0.136f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchL", "SpritesRyu.bmp", 86, 88, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchR", "SpritesRyu.bmp", 86 + 210, 88 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 87, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 88, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 87 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 88 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HKL", "SpritesRyu.bmp", 53, 57, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HKR", "SpritesRyu.bmp", 53 + 210, 57 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("HKL", 54, 0.068f);
	mainSpriteRender_->SetChangeFrame("HKL", 55, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKL", 56, 0.153f);
	mainSpriteRender_->SetChangeFrame("HKL", 57, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKR", 54 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HKR", 55 + 210, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKR", 56 + 210, 0.153f);
	mainSpriteRender_->SetChangeFrame("HKR", 57 + 210, 0.136f);

	mainSpriteRender_->CreateSpriteAnimation("HK_JumpL", "SpritesRyu.bmp", 98, 102, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_JumpR", "SpritesRyu.bmp", 98 + 210, 102 + 210, false, 0.034f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 99, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 100, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 101, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 99 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 100 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 101 + 210, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalL", "SpritesRyu.bmp", 104, 106, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalR", "SpritesRyu.bmp", 104 + 210, 106 + 210, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("HK_DiagonalL", 105, 0.119f);
	mainSpriteRender_->SetChangeFrame("HK_DiagonalR", 105 + 210, 0.119f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CloseL", "SpritesRyu.bmp", 67, 70, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CloseR", "SpritesRyu.bmp", 67 + 210, 70 + 210, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 69, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 70, 0.187f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 69 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 70 + 210, 0.187f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchL", "SpritesRyu.bmp", 88, 93, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchR", "SpritesRyu.bmp", 88 + 210, 93 + 210, false, 0.034f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 90, 0.102f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 91, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 92, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 93, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 90 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 91 + 210, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 92 + 210, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 93 + 210, 0.136f);

	// 필살기
	// 중 파동권
	mainSpriteRender_->CreateSpriteAnimation("HadoukenL", "SpritesRyu.bmp", 131, 135, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenR", "SpritesRyu.bmp", 131 + 210, 135 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HadoukenL", 132, 0.102f);
	mainSpriteRender_->SetChangeFrame("HadoukenR", 132 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("HadoukenL", 135, 0.663f);
	mainSpriteRender_->SetChangeFrame("HadoukenR", 135 + 210, 0.663f);

	// 강 승룡권
	mainSpriteRender_->CreateSpriteAnimation("ShoryukenL", "SpritesRyu.bmp", 108, 113, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("ShoryukenR", "SpritesRyu.bmp", 108 + 210, 113 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("ShoryukenL", 110, 0.442f);
	mainSpriteRender_->SetChangeFrame("ShoryukenL", 111, 0.238f);
	mainSpriteRender_->SetChangeFrame("ShoryukenL", 112, 0.221f);
	mainSpriteRender_->SetChangeFrame("ShoryukenL", 113, 0.085f);
	mainSpriteRender_->SetChangeFrame("ShoryukenR", 110 + 210, 0.442f);
	mainSpriteRender_->SetChangeFrame("ShoryukenR", 111 + 210, 0.238f);
	mainSpriteRender_->SetChangeFrame("ShoryukenR", 112 + 210, 0.221f);
	mainSpriteRender_->SetChangeFrame("ShoryukenR", 113 + 210, 0.085f);

	// 약 용권선풍각
	mainSpriteRender_->CreateSpriteAnimation("TatsumakiL", "SpritesRyu.bmp", 114, 130, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("TatsumakiR", "SpritesRyu.bmp", 114 + 210, 130 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 114, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 115, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 129, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiL", 130, 0.119f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 114 + 210, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 115 + 210, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 129 + 210, 0.085f);
	mainSpriteRender_->SetChangeFrame("TatsumakiR", 130 + 210, 0.119f);


	mainSpriteRender_->CreateSpriteAnimation("ThrowL", "SpritesRyu.bmp", 147 + 210, 151 + 210, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("ThrowR", "SpritesRyu.bmp", 147, 151, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("ThrowL", 147 + 210, 0.306f);
	mainSpriteRender_->SetChangeFrame("ThrowR", 147, 0.306f);
}

void Ryu::LastCheck()
{
	AnimationInit();
	dirstate_ = ACTORDIR::LOOK_LEFT;
	ChangeState(RyuState::Idle);
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	soundPlayer_->PlayCountReset(1);
	voicePlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	voicePlayer_->PlayCountReset(1);
}

void Ryu::ChangeState(RyuState _stateType)
{
	if (_stateType == RyuState::NONE)
	{
		return;
	}
	stateType_ = _stateType;

	switch (stateType_)
	{
	case RyuState::Idle:
		StateName = "Idle";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Walk:
		StateName = "Walk";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Crouch:
		StateName = "Crouch";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Jump:
		StateName = "Jump";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::JumpFront:
		StateName = "JumpFront";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::JumpBack:
		StateName = "JumpBack";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Guard:
		StateName = "Guard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::CrouchGuard:
		StateName = "CrouchGuard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HitUpperLight:
		StateName = "HitUpperLight";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HitUpperHeavy:
		StateName = "HitUpperHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HitDownLight:
		StateName = "HitDownLight";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HitDownHeavy:
		StateName = "HitDownHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HitWhenCrouch:
		StateName = "HitWhenCrouch";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HitWhenJump:
		StateName = "HitWhenJump";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HitWhenJumpDown:
		StateName = "HitWhenJumpDown";
		JumpDir = float4::DOWN * jumpForce_;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Down:
		StateName = "Down";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Recovery:
		StateName = "Recovery";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::GetUp:
		StateName = "GetUp";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::KO:
		StateName = "KO";
		JumpDir = float4::UP * jumpForce_ * 0.5f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::JumpKO:
		StateName = "HitWhenJump";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::R1Win:
		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			StateName = "R1Win";
			mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		}
		break;
	case RyuState::MatchPointWin:
		StateName = "MatchPointWin";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::StandDelay:
		StateName = "StandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::LandDelay:
		StateName = "LandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::LP:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::LP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::LP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::MP:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::MP_JumpBack:

		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::HP:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::HP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::HP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::HP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::HP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LK:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "LK_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::LK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::LK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MK:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "MK_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::MK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::MK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::MK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HK:
		strikeBox_->SetCollisionCount(1);
		StateName = "HK";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HK_Close:
		strikeBox_->SetCollisionCount(2);
		StateName = "HK_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "HK_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::HK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::HK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Hadouken:
		if (nullptr != projectile_)
		{
			break;
		}
		isShooting_ = false;
		strikeBox_->SetCollisionCount(0);
		StateName = "Hadouken";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Shoryuken:
		strikeBox_->SetCollisionCount(1);
		StateName = "Shoryuken";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Ryu_Shoryuken.mp3", 0);
		break;
	case RyuState::Tatsumaki:
		strikeBox_->SetCollisionCount(1);
		StateName = "Tatsumaki";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Ryu_Tatsumaki.mp3", 0);
		break;
	case RyuState::Throw:
		StateName = "Throw";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Ryu_Throw.mp3", 0);
		break;
	case RyuState::BeThrown:
		StateName = "BeThrown";
		JumpDir = float4::UP * jumpForce_ * 0.1f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::BeThrownDown:
		StateName = "BeThrownDown";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case RyuState::Burn:
		StateName = "Burn";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case RyuState::AIIdle:
		StateName = "Idle";
		mainSpriteRender_->ChangeSpriteAnimation("Idle" + GetDirString());
		break;
	case RyuState::AIWalk:
		StateName = "Walk";
		mainSpriteRender_->ChangeSpriteAnimation("Walk" + GetDirString());
		break;
	case RyuState::AICrouch:
		StateName = "Crouch";
		mainSpriteRender_->ChangeSpriteAnimation("Crouch" + GetDirString());
		break;
	case RyuState::AIJump:
		StateName = "Jump";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation("Jump" + GetDirString());
		break;
	case RyuState::AIJumpFront:
		StateName = "JumpFront";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation("JumpFront" + GetDirString());
		break;
	case RyuState::AIJumpBack:
		StateName = "JumpBack";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation("JumpBack" + GetDirString());
		break;
	default:
		break;
	}
}

void Ryu::UpdateBefore()
{
	ToggleDebug();
	ToggleAI();

	if (this->hp_ <= 0)
	{
		voicePlayer_->PlayOverlap("Ryu_KO.mp3", 0);
		if (true == mainSpriteRender_->IsCurSpriteAnimationName("BeThrownDown" + GetDirString()))
		{
			LeftRightCheck();
		}
		if (450.0f > GetPos().y)
		{
			return ChangeState(RyuState::JumpKO);

		}
		return ChangeState(RyuState::KO);
	}
}

std::string Ryu::GetDirString() // 업데이트로 체크된 "지향 방향" 을 측정해 글자를 리턴
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

void Ryu::LeftRightCheck() // "지향 방향" 을 체크해 스프라이트와 판정박스의 방향을 돌려준다.
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

void Ryu::Update()
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
		case RyuState::Idle:
			Idle();
			break;
		case RyuState::Walk:
			Walk();
			break;
		case RyuState::Crouch:
			Crouch();
			break;
		case RyuState::Jump:
			Jump();
			break;
		case RyuState::JumpFront:
			JumpFront();
			break;
		case RyuState::JumpBack:
			JumpBack();
			break;
		case RyuState::Guard:
			Guard();
			break;
		case RyuState::CrouchGuard:
			CrouchGuard();
			break;
		case RyuState::HitUpperLight:
			HitUpperLight();
			break;
		case RyuState::HitUpperHeavy:
			HitUpperHeavy();
			break;
		case RyuState::HitDownLight:
			HitDownLight();
			break;
		case RyuState::HitDownHeavy:
			HitDownHeavy();
			break;
		case RyuState::HitWhenCrouch:
			HitWhenCrouch();
			break;
		case RyuState::HitWhenJump:
			HitWhenJump();
			break;
		case RyuState::HitWhenJumpDown:
			HitWhenJumpDown();
			break;
		case RyuState::Down:
			Down();
			break;
		case RyuState::Recovery:
			Recovery();
			break;
		case RyuState::GetUp:
			GetUp();
			break;
		case RyuState::KO:
			KO();
			break;
		case RyuState::JumpKO:
			JumpKO();
			break;
		case RyuState::R1Win:
			Win();
			break;
		case RyuState::MatchPointWin:
			MatchPointWin();
			break;
		case RyuState::StandDelay:
			StandDelay();
			break;
		case RyuState::LandDelay:
			LandDelay();
			break;
		case RyuState::LP:
			LP();
			break;
		case RyuState::LP_Close:
			LP_Close();
			break;
		case RyuState::LP_Crouch:
			LP_Crouch();
			break;
		case RyuState::LP_Jump:
			LP_Jump();
			break;
		case RyuState::LP_JumpFront:
			LP_JumpFront();
			break;
		case RyuState::LP_JumpBack:
			LP_JumpBack();
			break;
		case RyuState::MP:
			MP();
			break;
		case RyuState::MP_Close:
			MP_Close();
			break;
		case RyuState::MP_Crouch:
			MP_Crouch();
			break;
		case RyuState::MP_Jump:
			MP_Jump();
			break;
		case RyuState::MP_JumpFront:
			MP_JumpFront();
			break;
		case RyuState::MP_JumpBack:
			MP_JumpBack();
			break;
		case RyuState::HP:
			HP();
			break;
		case RyuState::HP_Close:
			HP_Close();
			break;
		case RyuState::HP_Crouch:
			HP_Crouch();
			break;
		case RyuState::HP_Jump:
			HP_Jump();
			break;
		case RyuState::HP_JumpFront:
			HP_JumpFront();
			break;
		case RyuState::HP_JumpBack:
			HP_JumpBack();
			break;
		case RyuState::LK:
			LK();
			break;
		case RyuState::LK_Close:
			LK_Close();
			break;
		case RyuState::LK_Crouch:
			LK_Crouch();
			break;
		case RyuState::LK_Jump:
			LK_Jump();
			break;
		case RyuState::LK_JumpFront:
			LK_JumpFront();
			break;
		case RyuState::LK_JumpBack:
			LK_JumpBack();
			break;
		case RyuState::MK:
			MK();
			break;
		case RyuState::MK_Close:
			MK_Close();
			break;
		case RyuState::MK_Crouch:
			MK_Crouch();
			break;
		case RyuState::MK_Jump:
			MK_Jump();
			break;
		case RyuState::MK_JumpFront:
			MK_JumpFront();
			break;
		case RyuState::MK_JumpBack:
			MK_JumpBack();
			break;
		case RyuState::HK:
			HK();
			break;
		case RyuState::HK_Close:
			HK_Close();
			break;
		case RyuState::HK_Crouch:
			HK_Crouch();
			break;
		case RyuState::HK_Jump:
			HK_Jump();
			break;
		case RyuState::HK_JumpFront:
			HK_JumpFront();
			break;
		case RyuState::HK_JumpBack:
			HK_JumpBack();
			break;
		case RyuState::Hadouken:
			Hadouken();
			break;
		case RyuState::Shoryuken:
			Shoryuken();
			break;
		case RyuState::Tatsumaki:
			Tatsumaki();
			break;
		case RyuState::Throw:
			Throw();
			break;
		case RyuState::BeThrown:
			BeThrown();
			break;
		case RyuState::BeThrownDown:
			BeThrownDown();
			break;
		case RyuState::Burn:
			Burn();
			break;
		case RyuState::AIIdle:
			AIIdle();
			break;
		case RyuState::AIWalk:
			AIWalk();
			break;
		case RyuState::AICrouch:
			AICrouch();
			break;
		case RyuState::AIJump:
			AIJump();
			break;
		case RyuState::AIJumpFront:
			AIJumpFront();
			break;
		case RyuState::AIJumpBack:
			AIJumpBack();
			break;
		default:
			break;
		}
	}
}


void Ryu::Idle()
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
		return ChangeState(RyuState::AIIdle);
	}

	if (true == isSessionOver_)
	{
		return;
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") || true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		return ChangeState(RyuState::Walk);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(RyuState::Crouch);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		return ChangeState(RyuState::Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::LP_Close);
		}
		else
		{
			return ChangeState(RyuState::LP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::MP_Close);
		}
		else
		{
			return ChangeState(RyuState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::HP_Close);
		}
		else
		{
			return ChangeState(RyuState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::LK_Close);
		}
		else
		{
			return ChangeState(RyuState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::MK_Close);
		}
		else
		{
			return ChangeState(RyuState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::HK_Close);
		}
		else
		{
			return ChangeState(RyuState::HK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
	{
		return ChangeState(RyuState::Hadouken);
	}


}

void Ryu::Walk()
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
			return ChangeState(RyuState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{
		// 파동권이 존재한다는 것이니까.
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(RyuState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(RyuState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(RyuState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP") && nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{ // 잡기 시동
			switch (dirstate_)
			{
			case CharacterHandle::ACTORDIR::LOOK_RIGHT:
				return ChangeState(RyuState::Throw);
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
				return ChangeState(RyuState::JumpFront);
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(RyuState::JumpBack);
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
				return ChangeState(RyuState::Shoryuken); // 승룡권
				break;
			case ACTORDIR::LOOK_LEFT: // 왼 쪽을 바라보고 있으며 + 오른쪽 화살표키 + 필살기키를 누르면
				return ChangeState(RyuState::Tatsumaki); // 용권선풍각
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
				return ChangeState(RyuState::Throw);
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
				return ChangeState(RyuState::JumpBack);
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(RyuState::JumpFront);
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
				return ChangeState(RyuState::Tatsumaki); // 용권선풍각
				break;
			case ACTORDIR::LOOK_LEFT: // 왼 쪽을 바라보고 있으며 + 왼쪽 화살표키 + 필살기키를 누르면
				return ChangeState(RyuState::Shoryuken); // 승룡권
				break;
			default:
				break;
			}
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(RyuState::Crouch);
	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::LP_Close);
		}
		else
		{
			return ChangeState(RyuState::LP);
		}

	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::MP_Close);
		}
		else
		{
			return ChangeState(RyuState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::HP_Close);
		}
		else
		{
			return ChangeState(RyuState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::LK_Close);
		}
		else
		{
			return ChangeState(RyuState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::MK_Close);
		}
		else
		{
			return ChangeState(RyuState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(RyuState::HK_Close);
		}
		else
		{
			return ChangeState(RyuState::HK);
		}
	}

	if (
		false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right")
		&& false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left")
		)
	{
		return ChangeState(RyuState::Idle); // 꼭 return 으로 해주기.. 왜? if문 밖에 또 ChangeState() 있을 수 있으니...
	}


}

void Ryu::Crouch()
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
				return ChangeState(RyuState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
			ACTORDIR::LOOK_RIGHT == dirstate_)
		{
			// 파동권이 존재한다는 것이니까.
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(RyuState::CrouchGuard);
			}
		}

		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
		{
			if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
				|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
			{
				return ChangeState(RyuState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_)
		{
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(RyuState::CrouchGuard);
			}
		}


		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
		{
			return ChangeState(RyuState::LP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
		{
			return ChangeState(RyuState::MP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
		{
			return ChangeState(RyuState::HP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
		{
			return ChangeState(RyuState::LK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
		{
			return ChangeState(RyuState::MK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
		{
			return ChangeState(RyuState::HK_Crouch);
		}

		return ChangeState(RyuState::Crouch);
	}
	else if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(RyuState::Idle);
	}

}

void Ryu::Jump() // 수직점프
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(RyuState::LP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(RyuState::MP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(RyuState::HP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(RyuState::LK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(RyuState::MK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(RyuState::HK_Jump);
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
		if (450 <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetPos({ GetPos().x, 450.0f }); // 오차가 나올 때를 대비해 강제로 바닥 y 값에 맞춰 줍니다.
			isJump_ = false;
			return ChangeState(RyuState::LandDelay);
		}
	}
	return;
}

void Ryu::JumpFront()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(RyuState::LP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(RyuState::MP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(RyuState::HP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(RyuState::LK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(RyuState::MK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(RyuState::HK_JumpFront);
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
				return ChangeState(RyuState::LandDelay);
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
				return ChangeState(RyuState::LandDelay);
			}
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::JumpBack()
{
	ResetGuardBox();
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(RyuState::LP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(RyuState::MP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(RyuState::HP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(RyuState::LK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(RyuState::MK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(RyuState::HK_JumpBack);
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
				return ChangeState(RyuState::LandDelay);
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
				return ChangeState(RyuState::LandDelay);
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



void Ryu::LP()
{
	SetStrikeBox(mainSpriteRender_, 28, 29, 210, { 80,-45 }, { 128,30 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}

}

void Ryu::LP_Close()
{
	SetStrikeBox(mainSpriteRender_, 36, 37, 210, { 80,-70 }, { 80,35 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}


void Ryu::LP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 72, 73, 210, { 110, 15 }, { 110,35 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Crouch);
	}
}

void Ryu::LP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 50, -100 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
	}
}

void Ryu::LP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 50, -100 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::LP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 50, -100 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::MP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({20,15}) };
	hitBoxTorso_->SetSize({ 120, 210 });

	SetStrikeBox(mainSpriteRender_, 32, 33, 210, { 150, -65 }, { 105,65 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::MP_Close()
{
	SetStrikeBox(mainSpriteRender_, 40, 41, 210, { 100, -30 }, { 130,80 }, 22, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::MP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 75, 76, 210, { 120, 15 }, { 110,40 }, 22, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Crouch);
	}
}

void Ryu::MP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 70, -80 }, { 120,50 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
	}
}

void Ryu::MP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 70, -80 }, { 120,50 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::MP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 70, -80 }, { 120,50 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Ryu::HP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({30,15}) };
	hitBoxTorso_->SetSize({ 130, 210 });
	SetStrikeBox(mainSpriteRender_, 32, 33, 210, { 160, -65 }, { 115,75 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}


void Ryu::HP_Close()
{
	SetStrikeBox(mainSpriteRender_, 45, 46, 210, { 120, -30 }, { 130,100 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	SetStrikeBox(mainSpriteRender_, 46, 47, 210, { 120, -120 }, { 130,100 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (GetModifiedSpriteNumber(46) == mainSpriteRender_->GetCurAnimationFrame() || GetModifiedSpriteNumber(47) == mainSpriteRender_->GetCurAnimationFrame())
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			SetInFightText();
			ChangeState(RyuState::Shoryuken);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::HP_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorso_->SetSize({ 100,220 });
	SetStrikeBox(mainSpriteRender_, 79, 80, 210, { 100, -50 }, { 120,120 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	SetStrikeBox(mainSpriteRender_, 80, 81, 210, { 100, -90 }, { 90,170 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Crouch);
	}
}

void Ryu::HP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
	}
}

void Ryu::HP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::HP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 95, 96, 210, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::LK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });

	SetStrikeBox(mainSpriteRender_, 51, 52, 210, { 40, -80 }, { 160,85 }, 14, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::LK_Close()
{
	SetStrikeBox(mainSpriteRender_, 59, 60, 210, { 120, 50 }, { 160,110 }, 14, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::LK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 84, 85, 210, { 110, 115 }, { 160,55 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Crouch);
	}
}

void Ryu::LK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 210, { 20, -90 }, { 150,100 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
	}
}

void Ryu::LK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 103, 104, 210, { 20, -20 }, { 100,75 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::LK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 103, 104, 210, { 20, -20 }, { 100,75 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Ryu::MK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });
	SetStrikeBox(mainSpriteRender_, 51, 52, 210, { 40, -80 }, { 170,95 }, 24, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::MK_Close()
{
	SetStrikeBox(mainSpriteRender_, 63, 64, 210, { 100, -10 }, { 100,100 }, 24, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::MK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 87, 88, 210, { 160, 115 }, { 200,55 }, 22, HITSTYLE::DOWNHEAVY, "LOW");

	if (GetModifiedSpriteNumber(87) <= mainSpriteRender_->GetCurAnimationFrame() && GetModifiedSpriteNumber(89) >= mainSpriteRender_->GetCurAnimationFrame())
	{
			if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
			{
				SetInFightText();
				ChangeState(RyuState::Hadouken);
			}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Crouch);
	}
}

void Ryu::MK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 210, { 20, -90 }, { 150,100 }, 22, HITSTYLE::DOWNHEAVY, "MID");
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

			return ChangeState(RyuState::LandDelay);
		}
		return;
	}
}

void Ryu::MK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 210, { 90, 0 }, { 200,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
			return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::MK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 210, { 90, 0 }, { 200,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
				return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::HK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -10,35 }));
	hitBoxTorso_->SetSize({ 170, 185 });
	SetStrikeBox(mainSpriteRender_, 54, 55, 210, { 120, -80 }, { 140,105 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::HK_Close()
{
	SetStrikeBox(mainSpriteRender_, 68, 70, 210, { 100, -90 }, { 120,190 }, 26, HITSTYLE::HITUPPERHEAVY, "HIGH");
	SetStrikeBox(mainSpriteRender_, 69, 70, 210, { 100, -40 }, { 170,125 }, 4, HITSTYLE::DOWNHEAVY, "MID");
	// 애니메이션은 한 번 재생되고, 그 "재생" 시간동안 여기 있어야 한다면...
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::HK_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,60 }));
	hitBoxTorso_->SetSize({ 120, 165 });
	SetStrikeBoxLow(mainSpriteRender_, 90, 91, 210, { 155, 115 }, { 110,55 }, 30, HITSTYLE::DOWN, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Crouch);
	}
}

void Ryu::HK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 99, 100, 210, { 50, -50 }, { 140,100 }, 28, HITSTYLE::DOWNHEAVY, "MID");
	SetStrikeBoxOverHead(mainSpriteRender_, 100, 101, 210, { 50, -30 }, { 200,120 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
	}
}

void Ryu::HK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 210, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
			return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::HK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 105, 106, 210, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
				return ChangeState(RyuState::LandDelay);
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
			return ChangeState(RyuState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::Hadouken()
{
	if (GetModifiedSpriteNumber(135) == mainSpriteRender_->GetCurAnimationFrame())
	{

		hitBoxTorso_->SetPivot(SetModifiedHitbox({ 45,15 }));
		hitBoxTorso_->SetSize({ 200,190 });

		// projectile_ 이 파동권일 것이다.

		if (nullptr == projectile_ && false == isShooting_)
		{
			voicePlayer_->PlayAlone("Ryu_Hadouken.mp3", 0);
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
		return ChangeState(RyuState::Idle);
	}
}


void Ryu::Shoryuken() // 강 승룡권
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

	SetStrikeBox(mainSpriteRender_, 109, 110, 210, { 90,-130 }, { 120,190 }, 38, HITSTYLE::DOWN, "MID");
	// 지상에서 히트할 시 히트박스입니다.
	SetStrikeBox(mainSpriteRender_, 110, 111, 210, { 90,-130 }, { 120,190 }, 38, HITSTYLE::JUMP, "MID");
	// 대공 격추 히트박스입니다.
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (GetModifiedSpriteNumber(109) < mainSpriteRender_->GetCurAnimationFrame())
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
				return ChangeState(RyuState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (GetModifiedSpriteNumber(109) < mainSpriteRender_->GetCurAnimationFrame())
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

				return ChangeState(RyuState::LandDelay);
			}
		}
		return;
		break;
	default:
		break;
	}
}
void Ryu::Tatsumaki() // 용권선풍각
{ // 114~130
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 0,-100 }));
	hitBoxTorso_->SetSize({ 140,210 });
	SetStrikeBox(mainSpriteRender_, 116, 117, 210, { 90,-60 }, { 160,80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 118, 119, 210, { -90,-60 }, { 160, 80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 120, 121, 210, { 90,-60 }, { 160,80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 122, 123, 210, { -90,-60 }, { 160,80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 124, 125, 210, { 90,-60 }, { 160,80 }, 38, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 126, 127, 210, { -90,-60 }, { 160,80 }, 38, HITSTYLE::DOWN, "HIGH");

	if (GetModifiedSpriteNumber(130) > mainSpriteRender_->GetCurAnimationFrame())
	{
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (1000 >= GetPos().ix())
			{
				SetMove(float4::RIGHT * moveSpeed_ * 1.2f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (80 <= GetPos().ix())
			{
				SetMove(float4::LEFT * moveSpeed_ * 1.2f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
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
		return ChangeState(RyuState::LandDelay);
	}
}

void Ryu::Throw()
{
	RemoveCollisionBox();
	otherCharacter_->SetHold(33);
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::BeThrown()
{
	RemoveCollisionBox();
	if (GetModifiedSpriteNumber(205) < mainSpriteRender_->GetCurAnimationFrame())
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
			return ChangeState(RyuState::BeThrownDown);
		}

	}
}


void Ryu::BeThrownDown()
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
			return ChangeState(RyuState::GetUp);
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
			return ChangeState(RyuState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Ryu::SetHold(int _damage)
{
	ChangeState(RyuState::BeThrown);
}

// SetStrikeBox() : 타격 판정 박스를 특정 애니메이션 프레임에 활성화시킨 후 다음 프레임에 없애 주는 함수
// 상단기
void Ryu::SetStrikeBox(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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

// 중단기(공중 공격) 전용 스트라이크박스
void Ryu::SetStrikeBoxOverHead(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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
void Ryu::SetStrikeBoxLow(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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

void Ryu::ResetStrikeBox()
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

void Ryu::ResetGuardBox()
{
	guardBoxPivot_ = { 0, 2000 };
	guardBox_->SetPivot({ 0,2000 });
	guardBox_->SetSize({ 10,10 });
	crouchGuardBoxPivot_ = { 0, 2000 };
	crouchGuardBox_->SetPivot({ 0, 2000 });
	crouchGuardBox_->SetSize({ 10,10 });
}


void Ryu::ResetHitBox()
{
	//hitBoxHeadPivot_ = { 12, -75 };
	hitBoxTorsoPivot_ = { -10, 15 };
	//hitBoxLegPivot_ = { -8, 84 };
	//hitBoxHead_->SetSize({ 42,39 });
	hitBoxTorso_->SetSize({ 100,210 });
	//hitBoxLeg_->SetSize({ 100,105 });
}

void Ryu::ResetBodyBox()
{
	bodyBox_->SetPivot({ 0,15 });
	bodyBox_->SetSize({ 120,220 });
}

void Ryu::SetDamaged(int _damage, HITSTYLE _hitStyle)
{
	hp_ -= _damage; // 체력 차감
	if (stateType_ == RyuState::Crouch || stateType_ == RyuState::CrouchGuard) // 피격자가 앉아 있을 때는 무조건 앉은 상태의 피격 모션이 사용된다.
	{
		return ChangeState(RyuState::HitWhenCrouch);
	}

	if (stateType_ == RyuState::Shoryuken || stateType_ == RyuState::Tatsumaki)
	{
		return ChangeState(RyuState::HitWhenJump);
	}

	if (stateType_ == RyuState::Jump || stateType_ == RyuState::JumpBack || stateType_ == RyuState::JumpFront || stateType_ == RyuState::LP_Jump || stateType_ == RyuState::LP_JumpBack ||
		stateType_ == RyuState::LP_JumpFront || stateType_ == RyuState::MP_Jump || stateType_ == RyuState::MP_JumpBack || stateType_ == RyuState::MP_JumpFront || stateType_ == RyuState::HP_Jump ||
		stateType_ == RyuState::HP_JumpBack || stateType_ == RyuState::HP_JumpFront || stateType_ == RyuState::LK_Jump || stateType_ == RyuState::LK_JumpBack || stateType_ == RyuState::LK_JumpFront ||
		stateType_ == RyuState::MK_Jump || stateType_ == RyuState::MK_JumpBack || stateType_ == RyuState::MK_JumpFront || stateType_ == RyuState::HK_Jump || stateType_ == RyuState::HK_JumpBack || stateType_ == RyuState::HK_JumpFront ||
		stateType_ == RyuState::AIJump || stateType_ == RyuState::AIJumpFront || stateType_ == RyuState::AIJumpBack)
	{
		if (HITSTYLE::Burn == _hitStyle)
		{
			return ChangeState(RyuState::Burn);
		}
		return ChangeState(RyuState::HitWhenJump);
	}
	// 대미지 크기와 유형에 따른 모션 변경?
	switch (_hitStyle)
	{
	case CharacterHandle::HITSTYLE::HITUPPERLIGHT:
		return ChangeState(RyuState::HitUpperLight);
		break;
	case CharacterHandle::HITSTYLE::HITUPPERHEAVY:
		return ChangeState(RyuState::HitUpperHeavy);
		break;
	case CharacterHandle::HITSTYLE::DOWNLIGHT:
		return ChangeState(RyuState::HitDownLight);
		break;
	case CharacterHandle::HITSTYLE::DOWNHEAVY:
		return ChangeState(RyuState::HitDownHeavy);
		break;
	case CharacterHandle::HITSTYLE::CROUCH:
		return ChangeState(RyuState::HitWhenCrouch);
		break;
	case CharacterHandle::HITSTYLE::JUMP:
		return ChangeState(RyuState::HitWhenJump);
		break;
	case CharacterHandle::HITSTYLE::DOWN:
		return ChangeState(RyuState::Down);
		break;
	case CharacterHandle::HITSTYLE::BeThrownDown:
		return ChangeState(RyuState::BeThrownDown);
		break;
	case CharacterHandle::HITSTYLE::Burn:
		return ChangeState(RyuState::Burn);
	case CharacterHandle::HITSTYLE::MAX:
		break;
	default:
		break;
	}
}



void Ryu::HitUpperHeavy()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;

			if (GetModifiedSpriteNumber(169) < mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(169) < mainSpriteRender_->GetCurAnimationFrame())
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
			return ChangeState(RyuState::Idle);
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(169) < mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(169) < mainSpriteRender_->GetCurAnimationFrame())
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
			return ChangeState(RyuState::Idle);
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void Ryu::HitUpperLight()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			if (GetModifiedSpriteNumber(169) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(169) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(RyuState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(169) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(169) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(RyuState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ryu::HitDownHeavy()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			if (GetModifiedSpriteNumber(167) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(167) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(RyuState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(167) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(167) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(RyuState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ryu::HitDownLight()
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
			ChangeState(RyuState::Idle);
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
			ChangeState(RyuState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ryu::HitWhenCrouch()
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
			ChangeState(RyuState::Idle);
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
			ChangeState(RyuState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Ryu::HitWhenJump()
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
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(RyuState::HitWhenJumpDown);
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
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(RyuState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ryu::HitWhenJumpDown()
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
			return ChangeState(RyuState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f });
			isJump_ = false;
			return ChangeState(RyuState::GetUp);
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
			return ChangeState(RyuState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f }); // 오차가 나올 때를 대비해 강제로 바닥 y 값에 맞춰 줍니다.
			isJump_ = false;
			return ChangeState(RyuState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Ryu::Down()
{
	RemoveHitBox();
	RemoveBodyBox();
	ResetGuardBox();

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
			return ChangeState(RyuState::GetUp);
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
			ChangeState(RyuState::GetUp);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}


void Ryu::Burn()
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
			return ChangeState(RyuState::HitWhenJumpDown);
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
			return ChangeState(RyuState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Ryu::GetUp()
{
	RemoveHitBox();
	RemoveBodyBox();

	if (GetModifiedSpriteNumber(181) == mainSpriteRender_->GetCurAnimationFrame())
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			SetReversalText();
			return ChangeState(RyuState::Shoryuken);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::StandDelay);
	}
}


void Ryu::Recovery()
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
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::KO()
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
				if (knockBackDir_ <= 2.0f)
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
				if (knockBackDir_ <= 2.0f)
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

void Ryu::JumpKO()
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
			return ChangeState(RyuState::KO);
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
			return ChangeState(RyuState::KO);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void Ryu::Win()
{
	if ("P1" == GetName() && 1 <= FightScene::p1WinPoint_)
	{
		ChangeState(RyuState::MatchPointWin);
	}
	if ("P2" == GetName() && 1 <= FightScene::p2WinPoint_)
	{
		ChangeState(RyuState::MatchPointWin);
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}

void Ryu::MatchPointWin()
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}

void Ryu::StandDelay() // 4프레임의 기상 디스어드밴티지 -> 이 행동 동안은 가드밖에 할 수 없다.
{
	RemoveBodyBox();
	RemoveHitBox();
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(RyuState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{
		// 파동권이 존재한다는 것이니까.
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(RyuState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(RyuState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(RyuState::Guard);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}

void Ryu::LandDelay() // 점프 후 착지 시 딜레이
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(RyuState::Idle);
	}
}



void Ryu::Guard()
{
	ResetHitBox();
	ResetBodyBox();
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

	if (true == isAIOn_)
	{
		if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking()
			/* && false == otherCharacter_->GetProjectile()->IsOn() */)
		{
			ResetGuardBox();
			return ChangeState(RyuState::AIIdle);
		}
		if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking()
			/* && false == otherCharacter_->GetProjectile()->IsOn() */)
		{
			ResetGuardBox();
			return ChangeState(RyuState::AIIdle);
		}
		return;
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		ResetGuardBox();
		return ChangeState(RyuState::CrouchGuard);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		ResetGuardBox();
		return ChangeState(RyuState::JumpBack);
	}


	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Right"))
	{
		return ChangeState(RyuState::Idle);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Left"))
	{

		return ChangeState(RyuState::Idle);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR")
		/* && false == otherCharacter_->GetProjectile()->IsOn() */)
	{
		ResetGuardBox();
		return ChangeState(RyuState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL")
		/* && false == otherCharacter_->GetProjectile()->IsOn() */)
	{
		ResetGuardBox();
		return ChangeState(RyuState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
	{
		ResetGuardBox();
		return ChangeState(RyuState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		ResetGuardBox();
		return ChangeState(RyuState::Walk);
	}
}

void Ryu::CrouchGuard()
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

		return ChangeState(RyuState::Guard);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR"))
	{
		return ChangeState(RyuState::Crouch);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL"))
	{
		return ChangeState(RyuState::Crouch);
	}

}

void Ryu::UpdateAfter()
{
	if (0 >= otherCharacter_->GetCurHp())
	{

		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			if (true == mainSpriteRender_->IsCurSpriteAnimationName("Idle" + GetDirString()))
			{
				return ChangeState(RyuState::R1Win);
			}
			else if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				return ChangeState(RyuState::R1Win);
			}
		}
	}
}


void Ryu::Collision()
{


}



// *** AI

void Ryu::AIIdle()
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
		return ChangeState(RyuState::Idle);
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
				return ChangeState(RyuState::Guard);
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
				return ChangeState(RyuState::CrouchGuard);
			}
		}
	}

	if (nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		diceRoll_ = randomGenerator_.RandomInt(0, 2);
		if (diceRoll_ >= 1)
		{
			return ChangeState(RyuState::AIJumpFront);
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
				// 중거리 위치에서 전방 점프를 시도했으면 높은 확률로 대공 필살기나 대공 기본기를 쓸 수 있게 한다.
				return ChangeState(RyuState::Shoryuken);
			}
		}
		if (300.0f <= otherCharacter_->GetPos().x - GetPos().x || 300.0f <= GetPos().x - otherCharacter_->GetPos().x)
		{
			return ChangeState(RyuState::Hadouken);
		}
		diceRoll_ = randomGenerator_.RandomInt(0, 2);
		if (0 == diceRoll_)
		{
			return ChangeState(RyuState::MK_Crouch);
		}
		if (1 == diceRoll_)
		{
			return ChangeState(RyuState::HK);
		}
		if (2 == diceRoll_)
		{
			return ChangeState(RyuState::AIJumpFront);
		}

	}

	if (diceRoll_ == 4 || diceRoll_ == 5)
	{
		return ChangeState(RyuState::AIWalk);
	}
}

void Ryu::AIWalk()
{
	LeftRightCheck();
	switch (diceRoll_)
	{
	case 0:
	{
		return ChangeState(RyuState::AIIdle);
	}
	break;

	case 1:
	{
		return ChangeState(RyuState::AIIdle);
	}
	break;

	case 2:
	{
		return ChangeState(RyuState::AIIdle);
	}
	break;

	case 3:
	{
		return ChangeState(RyuState::AIIdle);
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
				return ChangeState(RyuState::MK_Crouch);
			}
			break;
			case 1:
			{
				return ChangeState(RyuState::AIJumpFront);
			}
			break;
			case 2:
			{
				return ChangeState(RyuState::HK);
			}
			break;
			case 3:
			{
				return ChangeState(RyuState::MK_Crouch);
			}
			break;
			default:
				break;
			}
			
		}
		else
		{
			return ChangeState(RyuState::AIIdle);
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
				return ChangeState(RyuState::MK_Crouch);
			}
			break;
			case 1:
			{
				return ChangeState(RyuState::AIJumpFront);
			}
			break;
			case 2:
			{
				return ChangeState(RyuState::HK);
			}
			break;
			case 3:
			{
				return ChangeState(RyuState::MK_Crouch);
			}
			break;
			default:
				break;
			}
		}
		else
		{
			return ChangeState(RyuState::AIIdle);
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
					return ChangeState(RyuState::AIJumpBack);
				}
				SetMove(float4::RIGHT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}

			if (GetPos().x - otherCharacter_->GetPos().x >= 400.0f)
			{
				return ChangeState(RyuState::AIIdle);
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
					return ChangeState(RyuState::AIJumpBack);
				}
				SetMove(float4::LEFT * moveSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			
			if (GetPos().x - otherCharacter_->GetPos().x <= -400.0f)
			{
				return ChangeState(RyuState::AIIdle);
			}
			return;
		}
	}
	break;
	default:
		break;
	}
}

void Ryu::AICrouch()
{

}

void Ryu::AIJump()
{

}

void Ryu::AIJumpFront()
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
				return ChangeState(RyuState::LK_JumpFront);
				break;
			case 1:
				return ChangeState(RyuState::MK_JumpFront);
				break;
			case 2:
				return ChangeState(RyuState::HP_JumpFront);
				break;
			case 3:
				return ChangeState(RyuState::MP_JumpFront);
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
				return ChangeState(RyuState::AIIdle);
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
				return ChangeState(RyuState::LK_JumpFront);

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
				return ChangeState(RyuState::AIIdle);
			}
		}
		return;
		break;
	default:
		break;
	}
}

void Ryu::AIJumpBack()
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
				return ChangeState(RyuState::AIIdle);
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
				return ChangeState(RyuState::Idle);
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