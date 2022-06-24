#include "Cammy.h"
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
#include "CharacterHandle.h"
#include <GameCollision.h>
#include "DustParticle.h"
#include "FightScene.h"
#include "FightEffects.h"

// Static Var
// Static Func

// constructer destructer
Cammy::Cammy()
	: stateType_(CammyState::MAX), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr)
{
	hp_ = 170;
	originalHp_ = 170;
	maxJumpHeight_ = 220.0f;
	moveSpeed_ = 400.0f;
	SetRenderOrder(10);
	jumpForce_ = 1300.0f;
}

Cammy::~Cammy()
{
}

Cammy::Cammy(Cammy&& _other) noexcept
	: stateType_(CammyState::MAX), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr)
{
}

//member Func
void Cammy::Render()
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

void Cammy::ToggleDebug()
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

void Cammy::KeyInit()
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

void Cammy::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("SpritesCammy.bmp");

	mainSpriteRender_->CreateSpriteAnimation("IdleL", "SpritesCammy.bmp", 0, 4, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("IdleR", "SpritesCammy.bmp", 0 + 220, 4 + 220, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("WalkL", "SpritesCammy.bmp", 5, 10, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("WalkR", "SpritesCammy.bmp", 5 + 220, 10 + 220, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("CrouchL", "SpritesCammy.bmp", 25, 25, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchR", "SpritesCammy.bmp", 25 + 220, 25 + 220, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("GuardL", "SpritesCammy.bmp", 46, 46, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardL", "SpritesCammy.bmp", 47, 47, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("GuardR", "SpritesCammy.bmp", 46 + 220, 46 + 220, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardR", "SpritesCammy.bmp", 47 + 220, 47 + 220, true, 0.1f);

	// 수직점프 애니메이션 설정
	mainSpriteRender_->CreateSpriteAnimation("JumpL", "SpritesCammy.bmp", 11, 16, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("JumpR", "SpritesCammy.bmp", 11 + 220, 16 + 220, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("JumpL", 13, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpL", 16, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpR", 13 + 220, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpR", 16 + 220, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("JumpFrontL", "SpritesCammy.bmp", 17, 23, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpFrontR", "SpritesCammy.bmp", 17 + 220, 23 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 17, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 18, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 22, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 23, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 17 + 220, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 18 + 220, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 22 + 220, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 23 + 220, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("JumpBackL", "SpritesCammy.bmp", 17, 23, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpBackR", "SpritesCammy.bmp", 17 + 220, 23 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 17, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 18, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 22, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 23, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 17 + 220, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 18 + 220, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 22 + 220, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 23 + 220, 0.204f);

	// 피격 & 다운 & 리커버리 & K.O. & 승리포즈
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightL", "SpritesCammy.bmp", 166, 166, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightR", "SpritesCammy.bmp", 166 + 220, 166 + 220, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyL", "SpritesCammy.bmp", 166, 169, false, 0.125f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyR", "SpritesCammy.bmp", 166 + 220, 169 + 220, false, 0.125f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownLightL", "SpritesCammy.bmp", 170, 170, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownLightR", "SpritesCammy.bmp", 170 + 220, 170 + 220, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyL", "SpritesCammy.bmp", 170, 173, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyR", "SpritesCammy.bmp", 170 + 220, 173 + 220, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchL", "SpritesCammy.bmp", 174, 174, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchR", "SpritesCammy.bmp", 174 + 220, 174 + 220, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpL", "SpritesCammy.bmp", 190, 190, false, 0.408f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpR", "SpritesCammy.bmp", 190 + 220, 190 + 220, false, 0.408f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownL", "SpritesCammy.bmp", 180, 182, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownR", "SpritesCammy.bmp", 180 + 220, 182 + 220, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownL", "SpritesCammy.bmp", 191, 194, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownR", "SpritesCammy.bmp", 191 + 220, 194 + 220, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownL", "SpritesCammy.bmp", 210, 213, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownR", "SpritesCammy.bmp", 210 + 220, 213 + 220, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("DownL", "SpritesCammy.bmp", 179, 182, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("DownR", "SpritesCammy.bmp", 179 + 182, 182 + 220, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("DownL", 182, 0.510f);
	mainSpriteRender_->SetChangeFrame("DownR", 182 + 220, 0.510f);

	mainSpriteRender_->CreateSpriteAnimation("BurnL", "SpritesCammy.bmp", 206, 208, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("BurnR", "SpritesCammy.bmp", 206 + 220, 208 + 220, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("RecoveryL", "SpritesCammy.bmp", 183, 189, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("RecoveryR", "SpritesCammy.bmp", 183 + 220, 189 + 220, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("GetUpL", "SpritesCammy.bmp", 184, 189, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("GetUpR", "SpritesCammy.bmp", 184 + 220, 189 + 220, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 184, 0.204f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 184 + 220, 0.204f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 189, 0.204f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 189 + 220, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("KOL", "SpritesCammy.bmp", 190, 194, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("KOR", "SpritesCammy.bmp", 190 + 220, 194 + 220, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("KOL", 194, 0.85f);
	mainSpriteRender_->SetChangeFrame("KOR", 194 + 220, 0.85f);

	mainSpriteRender_->CreateSpriteAnimation("R1WinL", "SpritesCammy.bmp", 195, 199, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("R1WinR", "SpritesCammy.bmp", 195 + 220, 199 + 220, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("R1WinL", 197, 0.510f);
	mainSpriteRender_->SetChangeFrame("R1WinR", 197 + 220, 0.510f);

	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinL", "SpritesCammy.bmp", 200, 204, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinR", "SpritesCammy.bmp", 200 + 220, 204 + 220, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("MatchPointWinL", 203, 0.510f);
	mainSpriteRender_->SetChangeFrame("MatchPointWinR", 203 + 220, 0.510f);

	mainSpriteRender_->CreateSpriteAnimation("StandDelayL", "SpritesCammy.bmp", 11, 11, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("StandDelayR", "SpritesCammy.bmp", 11 + 220, 11 + 220, false, 0.085f); // 5프레임 지연

	mainSpriteRender_->CreateSpriteAnimation("LandDelayL", "SpritesCammy.bmp", 11, 11, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LandDelayR", "SpritesCammy.bmp", 11+ 220, 11 + 220, false, 0.034f); // 2프레임 지연

	// 기본기
	mainSpriteRender_->CreateSpriteAnimation("LPL", "SpritesCammy.bmp", 27, 29, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LPR", "SpritesCammy.bmp", 27 + 220, 29 + 220, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("LP_CloseL", "SpritesCammy.bmp", 38, 40, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CloseR", "SpritesCammy.bmp", 38 + 220, 40 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 38, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 40, 0.085f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 38 + 220, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 40 + 220, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LP_JumpL", "SpritesCammy.bmp", 71, 72, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_JumpR", "SpritesCammy.bmp", 71 + 220, 72 + 220, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalL", "SpritesCammy.bmp", 71, 72, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalR", "SpritesCammy.bmp", 71 + 220, 72 + 220, false, 0.034f);

	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchL", "SpritesCammy.bmp", 93, 94, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchR", "SpritesCammy.bmp", 93 + 220, 94 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchL", 94, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchR", 94 + 220, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("MPL", "SpritesCammy.bmp", 30, 32, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MPR", "SpritesCammy.bmp", 30 + 220, 32 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MPL", 32, 0.102f);
	mainSpriteRender_->SetChangeFrame("MPR", 32 + 220, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("MP_JumpL", "SpritesCammy.bmp", 73, 75, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_JumpR", "SpritesCammy.bmp", 73 + 220, 75 + 220, false, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalL", "SpritesCammy.bmp", 73, 75, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalR", "SpritesCammy.bmp", 73 + 220, 75 + 220, false, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CloseL", "SpritesCammy.bmp", 30, 32, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CloseR", "SpritesCammy.bmp", 30 + 220, 32 + 220, false, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchL", "SpritesCammy.bmp", 90, 92, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchR", "SpritesCammy.bmp", 90 + 220, 92 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 91, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 92, 0.085f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 91 + 220, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 92 + 220, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HPL", "SpritesCammy.bmp", 34, 36, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HPR", "SpritesCammy.bmp", 34 + 220, 36 + 220, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("HPL", 36, 0.374f);
	mainSpriteRender_->SetChangeFrame("HPR", 36 + 220, 0.374f);

	mainSpriteRender_->CreateSpriteAnimation("HP_JumpL", "SpritesCammy.bmp", 76, 78, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_JumpR", "SpritesCammy.bmp", 76 + 220, 78 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_JumpL", 78, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_JumpR", 78 + 220, 0.34f);

	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalL", "SpritesCammy.bmp", 76, 78, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalR", "SpritesCammy.bmp", 76 + 220, 78 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalL", 78, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalR", 78 + 220, 0.34f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CloseL", "SpritesCammy.bmp", 40, 44, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CloseR", "SpritesCammy.bmp", 40 + 220, 44 + 220, false, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchL", "SpritesCammy.bmp", 95, 99, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchR", "SpritesCammy.bmp", 95 + 220, 99 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 98, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 99, 0.170f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 98 + 220, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 99 + 220, 0.170f);

	mainSpriteRender_->CreateSpriteAnimation("LKL", "SpritesCammy.bmp", 48, 50, false, 0.066f);
	mainSpriteRender_->CreateSpriteAnimation("LKR", "SpritesCammy.bmp", 48 + 220, 50 + 220, false, 0.066f);
	mainSpriteRender_->SetChangeFrame("LKL", 50, 0.085f);
	mainSpriteRender_->SetChangeFrame("LKR", 50 + 220, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_JumpL", "SpritesCammy.bmp", 79, 80, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpR", "SpritesCammy.bmp", 79 + 220, 80 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_JumpL", 80, 0.68f);
	mainSpriteRender_->SetChangeFrame("LK_JumpR", 80 + 220, 0.68f);

	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalL", "SpritesCammy.bmp", 79, 80, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalR", "SpritesCammy.bmp", 79 + 220, 80 + 220, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CloseL", "SpritesCammy.bmp", 61, 63, false, 0.066f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CloseR", "SpritesCammy.bmp", 61 + 220, 63 + 220, false, 0.066f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchL", "SpritesCammy.bmp", 100, 102, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchR", "SpritesCammy.bmp", 100 + 220, 102 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 101, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 102, 0.085f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 101 + 220, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 102 + 220, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("MKL", "SpritesCammy.bmp", 51, 55, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("MKR", "SpritesCammy.bmp", 51 + 220, 55 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("MKL", 55, 0.119f);
	mainSpriteRender_->SetChangeFrame("MKR", 55 + 220, 0.119f);

	mainSpriteRender_->CreateSpriteAnimation("MK_JumpL", "SpritesCammy.bmp", 81, 82, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_JumpR", "SpritesCammy.bmp", 81 + 220, 82 + 220, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_JumpL", 82, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_JumpR", 82 + 220, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalL", "SpritesCammy.bmp", 81, 82, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalR", "SpritesCammy.bmp", 81 + 220, 82 + 220, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalL", 82, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalR", 82 + 220, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CloseL", "SpritesCammy.bmp", 48, 50, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CloseR", "SpritesCammy.bmp", 48 + 220, 50 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CloseL", 50, 0.102f);
	mainSpriteRender_->SetChangeFrame("MK_CloseR", 50 + 220, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchL", "SpritesCammy.bmp", 104, 107, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchR", "SpritesCammy.bmp", 104 + 220, 107 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 105, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 107, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 105 + 220, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 107 + 220, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HKL", "SpritesCammy.bmp", 56, 60, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HKR", "SpritesCammy.bmp", 56 + 220, 60 + 220, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("HK_JumpL", "SpritesCammy.bmp", 83, 84, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_JumpR", "SpritesCammy.bmp", 83 + 220, 84 + 220, false, 0.034f);

	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalL", "SpritesCammy.bmp", 83, 84, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalR", "SpritesCammy.bmp", 83 + 220, 84 + 220, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CloseL", "SpritesCammy.bmp", 64, 70, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CloseR", "SpritesCammy.bmp", 64 + 220, 70 + 220, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 69, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 70, 0.187f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 69 + 220, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 70 + 220, 0.187f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchL", "SpritesCammy.bmp", 85, 89, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchR", "SpritesCammy.bmp", 85 + 220, 89 + 220, false, 0.034f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 88, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 89, 0.182f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 88 + 220, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 89 + 220, 0.182f);

	// 필살기
	// 중 스파이럴 애로우
	mainSpriteRender_->CreateSpriteAnimation("SpiralArrowL", "SpritesCammy.bmp", 108, 119, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("SpiralArrowR", "SpritesCammy.bmp", 108 + 220, 119 + 220, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("SpiralArrowL", 117, 0.102f);
	mainSpriteRender_->SetChangeFrame("SpiralArrowR", 117+220, 0.102f);
	mainSpriteRender_->SetChangeFrame("SpiralArrowL", 118, 0.102f);
	mainSpriteRender_->SetChangeFrame("SpiralArrowR", 118 + 220, 0.102f);
	mainSpriteRender_->SetChangeFrame("SpiralArrowL", 119, 0.204f);
	mainSpriteRender_->SetChangeFrame("SpiralArrowR", 119 + 220, 0.204f);

	// 강 캐논 스파이크
	mainSpriteRender_->CreateSpriteAnimation("CannonSpikeL", "SpritesCammy.bmp", 120, 126, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("CannonSpikeR", "SpritesCammy.bmp", 120 + 220, 126 + 220, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("CannonSpikeL", 120, 0.034f);
	mainSpriteRender_->SetChangeFrame("CannonSpikeR", 120 + 220, 0.034f);
	mainSpriteRender_->SetChangeFrame("CannonSpikeL", 121, 0.034f);
	mainSpriteRender_->SetChangeFrame("CannonSpikeR", 121 + 220, 0.034f);
	mainSpriteRender_->SetChangeFrame("CannonSpikeL", 123, 0.306f);
	mainSpriteRender_->SetChangeFrame("CannonSpikeR", 123 + 220, 0.306f);
	// 강 스피닝 백 너클
	mainSpriteRender_->CreateSpriteAnimation("SpinBackKnuckleL", "SpritesCammy.bmp", 128, 137, true, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("SpinBackKnuckleR", "SpritesCammy.bmp", 128 + 220, 137 + 220, true, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("ThrowL", "SpritesCammy.bmp", 138 + 220, 141 + 220, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("ThrowR", "SpritesCammy.bmp", 138, 141, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("ThrowL", 138 + 220, 0.306f);
	mainSpriteRender_->SetChangeFrame("ThrowR", 138, 0.306f);
	mainSpriteRender_->SetChangeFrame("ThrowL", 139 + 220, 0.102f);
	mainSpriteRender_->SetChangeFrame("ThrowR", 139, 0.102f);



}

void Cammy::LastCheck()
{
	AnimationInit();
	dirstate_ = ACTORDIR::LOOK_LEFT;
	ChangeState(CammyState::Idle);
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	soundPlayer_->PlayCountReset(1);
	voicePlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	voicePlayer_->PlayCountReset(1);
}

void Cammy::ChangeState(CammyState _stateType)
{
	if (_stateType == CammyState::NONE)
	{
		return;
	}
	stateType_ = _stateType;

	switch (stateType_)
	{
	case CammyState::Idle:
		StateName = "Idle";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Walk:
		StateName = "Walk";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Crouch:
		StateName = "Crouch";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Jump:
		StateName = "Jump";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::JumpFront:
		StateName = "JumpFront";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::JumpBack:
		StateName = "JumpBack";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Guard:
		StateName = "Guard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::CrouchGuard:
		StateName = "CrouchGuard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HitUpperLight:
		StateName = "HitUpperLight";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HitUpperHeavy:
		StateName = "HitUpperHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HitDownLight:
		StateName = "HitDownLight";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HitDownHeavy:
		StateName = "HitDownHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HitWhenCrouch:
		StateName = "HitWhenCrouch";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HitWhenJump:
		StateName = "HitWhenJump";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HitWhenJumpDown:
		StateName = "HitWhenJumpDown";
		JumpDir = float4::DOWN * jumpForce_;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Down:
		StateName = "Down";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Recovery:
		StateName = "Recovery";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::GetUp:
		StateName = "GetUp";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::KO:
		StateName = "KO";
		JumpDir = float4::UP * jumpForce_ * 0.5f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::JumpKO:
		StateName = "HitWhenJump";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::R1Win:
		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			StateName = "R1Win";
			mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		}
		break;
	case CammyState::MatchPointWin:
		StateName = "MatchPointWin";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::StandDelay:
		StateName = "StandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::LandDelay:
		StateName = "LandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::LP:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::LP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::LP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::MP:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::MP_JumpBack:

		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::HP:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::HP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::HP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::HP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::HP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LK:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "LK_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::LK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::LK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MK:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "MK_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::MK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::MK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::MK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HK:
		strikeBox_->SetCollisionCount(1);
		StateName = "HK";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "HK_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "HK_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case CammyState::HK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::HK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::SpiralArrow:
		strikeBox_->SetCollisionCount(1);
		StateName = "SpiralArrow";
		voicePlayer_->PlayOverlap("Cammy_SpiralArrow.mp3", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::CannonSpike:
		strikeBox_->SetCollisionCount(1);
		StateName = "CannonSpike";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Cammy_CannonSpike.mp3", 0);
		break;
	case CammyState::SpinBackKnuckle:
		strikeBox_->SetCollisionCount(1);
		StateName = "SpinBackKnuckle";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Throw:
		StateName = "Throw";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("Cammy_Throw.mp3", 0);
		break;
	case CammyState::BeThrown:
		StateName = "BeThrown";
		JumpDir = float4::UP * jumpForce_ * 0.1f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::BeThrownDown:
		StateName = "BeThrownDown";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case CammyState::Burn:
		StateName = "Burn";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	default:
		break;
	}
}

void Cammy::UpdateBefore()
{
	ToggleDebug();

	if (this->hp_ <= 0)
	{
		voicePlayer_->PlayOverlap("Cammy_KO.mp3", 0);
		if (true == mainSpriteRender_->IsCurSpriteAnimationName("BeThrownDown" + GetDirString()))
		{
			LeftRightCheck();
		}
		if (450.0f > GetPos().y)
		{
			return ChangeState(CammyState::JumpKO);

		}
		return ChangeState(CammyState::KO);
	}
}


std::string Cammy::GetDirString() // 업데이트로 체크된 "지향 방향" 을 측정해 글자를 리턴
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

void Cammy::LeftRightCheck() // "지향 방향" 을 체크해 스프라이트와 판정박스의 방향을 돌려준다.
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

void Cammy::Update()
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
		case CammyState::Idle:
			Idle();
			break;
		case CammyState::Walk:
			Walk();
			break;
		case CammyState::Crouch:
			Crouch();
			break;
		case CammyState::Jump:
			Jump();
			break;
		case CammyState::JumpFront:
			JumpFront();
			break;
		case CammyState::JumpBack:
			JumpBack();
			break;
		case CammyState::Guard:
			Guard();
			break;
		case CammyState::CrouchGuard:
			CrouchGuard();
			break;
		case CammyState::HitUpperLight:
			HitUpperLight();
			break;
		case CammyState::HitUpperHeavy:
			HitUpperHeavy();
			break;
		case CammyState::HitDownLight:
			HitDownLight();
			break;
		case CammyState::HitDownHeavy:
			HitDownHeavy();
			break;
		case CammyState::HitWhenCrouch:
			HitWhenCrouch();
			break;
		case CammyState::HitWhenJump:
			HitWhenJump();
			break;
		case CammyState::HitWhenJumpDown:
			HitWhenJumpDown();
			break;
		case CammyState::Down:
			Down();
			break;
		case CammyState::Recovery:
			Recovery();
			break;
		case CammyState::GetUp:
			GetUp();
			break;
		case CammyState::KO:
			KO();
			break;
		case CammyState::JumpKO:
			JumpKO();
			break;
		case CammyState::R1Win:
			Win();
			break;
		case CammyState::MatchPointWin:
			MatchPointWin();
			break;
		case CammyState::StandDelay:
			StandDelay();
			break;
		case CammyState::LandDelay:
			LandDelay();
			break;
		case CammyState::LP:
			LP();
			break;
		case CammyState::LP_Close:
			LP_Close();
			break;
		case CammyState::LP_Crouch:
			LP_Crouch();
			break;
		case CammyState::LP_Jump:
			LP_Jump();
			break;
		case CammyState::LP_JumpFront:
			LP_JumpFront();
			break;
		case CammyState::LP_JumpBack:
			LP_JumpBack();
			break;
		case CammyState::MP:
			MP();
			break;
		case CammyState::MP_Close:
			MP_Close();
			break;
		case CammyState::MP_Crouch:
			MP_Crouch();
			break;
		case CammyState::MP_Jump:
			MP_Jump();
			break;
		case CammyState::MP_JumpFront:
			MP_JumpFront();
			break;
		case CammyState::MP_JumpBack:
			MP_JumpBack();
			break;
		case CammyState::HP:
			HP();
			break;
		case CammyState::HP_Close:
			HP_Close();
			break;
		case CammyState::HP_Crouch:
			HP_Crouch();
			break;
		case CammyState::HP_Jump:
			HP_Jump();
			break;
		case CammyState::HP_JumpFront:
			HP_JumpFront();
			break;
		case CammyState::HP_JumpBack:
			HP_JumpBack();
			break;
		case CammyState::LK:
			LK();
			break;
		case CammyState::LK_Close:
			LK_Close();
			break;
		case CammyState::LK_Crouch:
			LK_Crouch();
			break;
		case CammyState::LK_Jump:
			LK_Jump();
			break;
		case CammyState::LK_JumpFront:
			LK_JumpFront();
			break;
		case CammyState::LK_JumpBack:
			LK_JumpBack();
			break;
		case CammyState::MK:
			MK();
			break;
		case CammyState::MK_Close:
			MK_Close();
			break;
		case CammyState::MK_Crouch:
			MK_Crouch();
			break;
		case CammyState::MK_Jump:
			MK_Jump();
			break;
		case CammyState::MK_JumpFront:
			MK_JumpFront();
			break;
		case CammyState::MK_JumpBack:
			MK_JumpBack();
			break;
		case CammyState::HK:
			HK();
			break;
		case CammyState::HK_Close:
			HK_Close();
			break;
		case CammyState::HK_Crouch:
			HK_Crouch();
			break;
		case CammyState::HK_Jump:
			HK_Jump();
			break;
		case CammyState::HK_JumpFront:
			HK_JumpFront();
			break;
		case CammyState::HK_JumpBack:
			HK_JumpBack();
			break;
		case CammyState::SpiralArrow:
			SpiralArrow();
			break;
		case CammyState::CannonSpike:
			CannonSpike();
			break;
		case CammyState::SpinBackKnuckle:
			SpinBackKnuckle();
			break;
		case CammyState::Throw:
			Throw();
			break;
		case CammyState::BeThrown:
			BeThrown();
			break;
		case CammyState::BeThrownDown:
			BeThrownDown();
			break;
		case CammyState::Burn:
			Burn();
			break;
		default:
			break;
		}
	}
}

void Cammy::Idle()
{
	soundPlayer_->PlayCountReset(1);
	voicePlayer_->PlayCountReset(1);
	ResetStrikeBox();
	ResetGuardBox();
	ResetHitBox();
	ResetBodyBox();
	ResetCollisionBox();
	SetPos({ GetPos().x, 450.0f });

	isAttacking_ = false;
	isDown_ = false;

	curDirectionX_ = 0;
	curDirectionY_ = 0;

	LeftRightCheck();

	if (true == isSessionOver_)
	{
		return;
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") || true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		return ChangeState(CammyState::Walk);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(CammyState::Crouch);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		return ChangeState(CammyState::Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::LP_Close);
		}
		else
		{
			return ChangeState(CammyState::LP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::MP_Close);
		}
		else
		{
			return ChangeState(CammyState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::HP_Close);
		}
		else
		{
			return ChangeState(CammyState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::LK_Close);
		}
		else
		{
			return ChangeState(CammyState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::MK_Close);
		}
		else
		{
			return ChangeState(CammyState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::HK_Close);
		}
		else
		{
			return ChangeState(CammyState::HK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
	{
		return ChangeState(CammyState::SpiralArrow);
	}
}

void Cammy::Walk()
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
			return ChangeState(CammyState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{

		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(CammyState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(CammyState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(CammyState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP") && nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{ // 잡기 시동
			switch (dirstate_)
			{
			case CharacterHandle::ACTORDIR::LOOK_RIGHT:
				return ChangeState(CammyState::Throw);
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
				return ChangeState(CammyState::JumpFront);
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(CammyState::JumpBack);
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
				return ChangeState(CammyState::CannonSpike); // 승룡권
				break;
			case ACTORDIR::LOOK_LEFT: // 왼 쪽을 바라보고 있으며 + 오른쪽 화살표키 + 필살기키를 누르면
				return ChangeState(CammyState::SpinBackKnuckle); // 용권선풍각
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
				return ChangeState(CammyState::Throw);
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
				return ChangeState(CammyState::JumpBack);
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(CammyState::JumpFront);
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
				return ChangeState(CammyState::SpinBackKnuckle); // 용권선풍각
				break;
			case ACTORDIR::LOOK_LEFT: // 왼 쪽을 바라보고 있으며 + 왼쪽 화살표키 + 필살기키를 누르면
				return ChangeState(CammyState::CannonSpike); // 승룡권
				break;
			default:
				break;
			}
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(CammyState::Crouch);
	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::LP_Close);
		}
		else
		{
			return ChangeState(CammyState::LP);
		}

	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::MP_Close);
		}
		else
		{
			return ChangeState(CammyState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::HP_Close);
		}
		else
		{
			return ChangeState(CammyState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::LK_Close);
		}
		else
		{
			return ChangeState(CammyState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::MK_Close);
		}
		else
		{
			return ChangeState(CammyState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(CammyState::HK_Close);
		}
		else
		{
			return ChangeState(CammyState::HK);
		}
	}

	if (
		false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right")
		&& false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left")
		)
	{
		return ChangeState(CammyState::Idle); // 꼭 return 으로 해주기.. 왜? if문 밖에 또 ChangeState() 있을 수 있으니...
	}


}

void Cammy::Crouch()
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
				return ChangeState(CammyState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
			ACTORDIR::LOOK_RIGHT == dirstate_)
		{
			// 파동권이 존재한다는 것이니까.
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(CammyState::CrouchGuard);
			}
		}

		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
		{
			if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
				|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
			{
				return ChangeState(CammyState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_)
		{
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(CammyState::CrouchGuard);
			}
		}


		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
		{
			return ChangeState(CammyState::LP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
		{
			return ChangeState(CammyState::MP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
		{
			return ChangeState(CammyState::HP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
		{
			return ChangeState(CammyState::LK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
		{
			return ChangeState(CammyState::MK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
		{
			return ChangeState(CammyState::HK_Crouch);
		}

		return ChangeState(CammyState::Crouch);
	}
	else if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(CammyState::Idle);
	}

}

void Cammy::Jump() // 수직점프
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(CammyState::LP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(CammyState::MP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(CammyState::HP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(CammyState::LK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(CammyState::MK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(CammyState::HK_Jump);
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
			return ChangeState(CammyState::LandDelay);
		}
	}
	return;
}

void Cammy::JumpFront()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(CammyState::LP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(CammyState::MP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(CammyState::HP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(CammyState::LK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(CammyState::MK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(CammyState::HK_JumpFront);
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
				return ChangeState(CammyState::LandDelay);
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
				return ChangeState(CammyState::LandDelay);
			}
		}
		return;
		break;
	default:
		break;
	}

}

void Cammy::JumpBack()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(CammyState::LP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(CammyState::MP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(CammyState::HP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(CammyState::LK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(CammyState::MK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(CammyState::HK_JumpBack);
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
				return ChangeState(CammyState::LandDelay);
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
				return ChangeState(CammyState::LandDelay);
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

void Cammy::LP()
{
	SetStrikeBox(mainSpriteRender_, 28, 29, 220, { 110,-55 }, { 140,30 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}

}

void Cammy::LP_Close()
{
	SetStrikeBox(mainSpriteRender_, 39, 40, 220, { 80,-30 }, { 80,80 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}


void Cammy::LP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 94, 95, 220, { 120, 30 }, { 110,35 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Crouch);
	}
}

void Cammy::LP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 72, 73, 220, { 70, -70 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
	}
}

void Cammy::LP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 72, 73, 220, { 70, -70 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::LP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 72, 73, 220, { 70, -70 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::MP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({20,15}) };
	hitBoxTorso_->SetSize({ 120, 220 });

	SetStrikeBox(mainSpriteRender_, 31, 32, 220, { 150, -65 }, { 105,65 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::MP_Close()
{
	SetStrikeBox(mainSpriteRender_, 31, 32, 220, { 100, -30 }, { 130,80 }, 22, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::MP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 91, 92, 220, { 120, 20 }, { 130,60 }, 22, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Crouch);
	}
}

void Cammy::MP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 74, 75, 220, { 70, -50 }, { 120,90 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
	}
}

void Cammy::MP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 74, 75, 220, { 70, -50 }, { 120,90 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::MP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 74, 75, 220, { 70, -50 }, { 120,90 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Cammy::HP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({30,15}) };
	hitBoxTorso_->SetSize({ 130, 220 });
	SetStrikeBox(mainSpriteRender_, 35, 36, 220, { 160, -50 }, { 115,85 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}


void Cammy::HP_Close()
{
	SetStrikeBox(mainSpriteRender_, 43, 44, 220, { 120, -90 }, { 100,100 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (GetModifiedSpriteNumber(43) == mainSpriteRender_->GetCurAnimationFrame() || GetModifiedSpriteNumber(44) == mainSpriteRender_->GetCurAnimationFrame())
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			SetInFightText();
			ChangeState(CammyState::CannonSpike);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::HP_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorso_->SetSize({ 100,220 });
	SetStrikeBox(mainSpriteRender_, 98, 99, 220, { 150, -40 }, { 100 ,130 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Crouch);
	}
}

void Cammy::HP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 77, 78, 220, { 70, -10 }, { 130,60 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
	}
}

void Cammy::HP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 77, 78, 220, { 70, -10 }, { 130,60 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::HP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 77, 78, 220, { 70, -10 }, { 130,60 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::LK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });

	SetStrikeBox(mainSpriteRender_, 49, 50, 220, { 50, -80 }, { 160,65 }, 14, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::LK_Close()
{
	SetStrikeBox(mainSpriteRender_, 62, 63, 220, { 120, 30 }, { 160,110 }, 14, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::LK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 101, 102, 220, { 90, 115 }, { 160,55 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Crouch);
	}
}

void Cammy::LK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 80, 81, 220, { 130, -20 }, { 100,75 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
	}
}

void Cammy::LK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 80, 81, 220, { 130, -20 }, { 100,75 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::LK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 80, 81, 220, { 130, -20 }, { 100,75 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void Cammy::MK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });
	SetStrikeBox(mainSpriteRender_, 53, 54, 220, { 160, -20 }, { 170,95 }, 24, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::MK_Close()
{
	SetStrikeBox(mainSpriteRender_, 49, 50, 220, { 100, -10 }, { 100,100 }, 24, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::MK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 105, 106, 220, { 160, 115 }, { 200,55 }, 22, HITSTYLE::DOWNHEAVY, "LOW");

	if (GetModifiedSpriteNumber(87) <= mainSpriteRender_->GetCurAnimationFrame() && GetModifiedSpriteNumber(89) >= mainSpriteRender_->GetCurAnimationFrame())
	{
		if (InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			return ChangeState(CammyState::CannonSpike);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Crouch);
	}
}

void Cammy::MK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 82, 83, 220, { 140, -120 }, { 200,55 }, 22, HITSTYLE::DOWNHEAVY, "MID");
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

			return ChangeState(CammyState::LandDelay);
		}
		return;
	}
}

void Cammy::MK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 82, 83, 220, { 140, -120 }, { 200,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
			return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::MK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 82, 83, 220, { 140, -120 }, { 200,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (17 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
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
				return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::HK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -10,35 }));
	hitBoxTorso_->SetSize({ 170, 185 });
	SetStrikeBox(mainSpriteRender_, 59, 60, 220, { 140, -40 }, { 140,105 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::HK_Close()
{
	SetStrikeBox(mainSpriteRender_, 67, 68, 220, { 100, -40 }, { 120,190 }, 26, HITSTYLE::HITUPPERHEAVY, "HIGH");
	// 애니메이션은 한 번 재생되고, 그 "재생" 시간동안 여기 있어야 한다면...
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::HK_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,60 }));
	hitBoxTorso_->SetSize({ 120, 165 });
	SetStrikeBoxLow(mainSpriteRender_, 87, 88, 220, { 155, 115 }, { 110,55 }, 30, HITSTYLE::DOWN, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Crouch);
	}
}

void Cammy::HK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 84, 85, 220, { 120, -40 }, { 150,55 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
	}
}

void Cammy::HK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 84, 85, 220, { 120, -40 }, { 150,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
			return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::HK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 84, 85, 220, { 120, -40 }, { 150,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (17 < mainSpriteRender_->GetCurAnimationFrame()) // 16번 애니메이션이 끝날 때 점프가 시작됩니다.
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
				return ChangeState(CammyState::LandDelay);
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
			return ChangeState(CammyState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::SpiralArrow()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,60 }));
	hitBoxTorsoPivot_ = { 20, 60 };
	hitBoxTorso_->SetSize({ 100, 155 });
	SetStrikeBox(mainSpriteRender_, 110, 117, 220, { 90,-20 }, { 160,80 }, 38, HITSTYLE::DOWN, "LOW");

	if (GetModifiedSpriteNumber(109) < mainSpriteRender_->GetCurAnimationFrame() && GetModifiedSpriteNumber(116) >= mainSpriteRender_->GetCurAnimationFrame())
	{
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (1000 >= GetPos().ix())
			{
				SetMove(float4::RIGHT * moveSpeed_ * 1.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (80 <= GetPos().ix())
			{
				SetMove(float4::LEFT * moveSpeed_ * 1.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::MAX:
			break;
		default:
			break;
		}
	}

	if (GetModifiedSpriteNumber(116) < mainSpriteRender_->GetCurAnimationFrame())
	{
		
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (1000 >= GetPos().ix())
			{
				SetMove(float4::RIGHT * moveSpeed_ * 0.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (80 <= GetPos().ix())
			{
				SetMove(float4::LEFT * moveSpeed_ * 0.5f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::MAX:
			break;
		default:
			break;
		}
	}
	if (GetModifiedSpriteNumber(116) == mainSpriteRender_->GetCurAnimationFrame())
	{
		SetDustParticle();
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		
		return ChangeState(CammyState::LandDelay);
	}
}

void Cammy::CannonSpike()
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

	SetStrikeBox(mainSpriteRender_, 122, 124, 220, { 90,-80 }, { 120,190 }, 38, HITSTYLE::DOWN, "MID");
	switch (dirstate_)
	{
	case ACTORDIR::LOOK_RIGHT:
		if (GetModifiedSpriteNumber(122) < mainSpriteRender_->GetCurAnimationFrame())
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
				return ChangeState(CammyState::LandDelay);
			}
		}
		return;
		break;
	case ACTORDIR::LOOK_LEFT:
		if (GetModifiedSpriteNumber(122) < mainSpriteRender_->GetCurAnimationFrame())
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
				return ChangeState(CammyState::LandDelay);
			}
		}
		return;
		break;
	default:
		break;
	}
}

void Cammy::SpinBackKnuckle()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,60 }));
	hitBoxTorsoPivot_ = { 20, 60 };
	hitBoxTorso_->SetSize({ 100, 155 });
	SetStrikeBox(mainSpriteRender_, 135, 136, 220, { 120, -20 }, { 160,80 }, 30, HITSTYLE::DOWN, "MID");
	if (GetModifiedSpriteNumber(134) == mainSpriteRender_->GetCurAnimationFrame())
	{
		voicePlayer_->PlayOverlap("Cammy_Throw.mp3", 0);
	}

	if (GetModifiedSpriteNumber(129) <= mainSpriteRender_->GetCurAnimationFrame() && GetModifiedSpriteNumber(133) >= mainSpriteRender_->GetCurAnimationFrame())
	{
		RemoveHitBox();
		RemoveBodyBox();
		ResetGuardBox();
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (1000 >= GetPos().ix())
			{
				SetMove(float4::RIGHT * moveSpeed_ * 2.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (80 <= GetPos().ix())
			{
				SetMove(float4::LEFT * moveSpeed_ * 2.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
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
		return ChangeState(CammyState::LandDelay);
	}
}

void Cammy::Throw()
{
	RemoveCollisionBox();
	otherCharacter_->SetHold(33);
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::GetUp);
	}
}

void Cammy::BeThrown()
{
	RemoveCollisionBox();
	if (GetModifiedSpriteNumber(210) < mainSpriteRender_->GetCurAnimationFrame())
	{
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			if (1000.0f >= GetCameraLinkedPos().x)
			{
				SetMove(float4::RIGHT * knockBackDir_ * 1.8f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 <= knockBackDir_)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 200.0f;
			}
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			if (152.0f <= GetCameraLinkedPos().x)
			{
				SetMove(float4::LEFT * knockBackDir_ * 1.8f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			}
			if (0 <= knockBackDir_)
			{
				knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 200.0f;
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
			return ChangeState(CammyState::BeThrownDown);
		}

	}
}


void Cammy::BeThrownDown()
{
	isDown_ = true;
	RemoveHitBox();
	RemoveBodyBox();
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
			return ChangeState(CammyState::GetUp);
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
			return ChangeState(CammyState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Cammy::SetHold(int _damage)
{
	ChangeState(CammyState::BeThrown);
}

// SetStrikeBox() : 타격 판정 박스를 특정 애니메이션 프레임에 활성화시킨 후 다음 프레임에 없애 주는 함수
// 상단기
void Cammy::SetStrikeBox(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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
void Cammy::SetStrikeBoxOverHead(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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
void Cammy::SetStrikeBoxLow(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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

void Cammy::ResetStrikeBox()
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

void Cammy::ResetGuardBox()
{
	guardBoxPivot_ = { 0, 2000 };
	guardBox_->SetPivot({ 0,2000 });
	guardBox_->SetSize({ 10,10 });
	crouchGuardBoxPivot_ = { 0, 2000 };
	crouchGuardBox_->SetPivot({ 0, 2000 });
	crouchGuardBox_->SetSize({ 10,10 });
}

void Cammy::ResetHitBox()
{
	//hitBoxHeadPivot_ = { 12, -75 };
	hitBoxTorsoPivot_ = { -10, 15 };
	//hitBoxLegPivot_ = { -8, 84 };
	//hitBoxHead_->SetSize({ 42,39 });
	hitBoxTorso_->SetSize({ 100,210 });
	//hitBoxLeg_->SetSize({ 100,105 });
}

void Cammy::ResetBodyBox()
{
	bodyBox_->SetPivot({ 0,15 });
	bodyBox_->SetSize({ 120,220 });
}


void Cammy::SetDamaged(int _damage, HITSTYLE _hitStyle)
{
	hp_ -= _damage; // 체력 차감
	if (stateType_ == CammyState::Crouch || stateType_ == CammyState::CrouchGuard) // 피격자가 앉아 있을 때는 무조건 앉은 상태의 피격 모션이 사용된다.
	{
		return ChangeState(CammyState::HitWhenCrouch);
	}

	if (stateType_ == CammyState::CannonSpike || stateType_ == CammyState::SpiralArrow)
	{
		return ChangeState(CammyState::HitWhenJump);
	}

	if (stateType_ == CammyState::Jump || stateType_ == CammyState::JumpBack || stateType_ == CammyState::JumpFront || stateType_ == CammyState::LP_Jump || stateType_ == CammyState::LP_JumpBack ||
		stateType_ == CammyState::LP_JumpFront || stateType_ == CammyState::MP_Jump || stateType_ == CammyState::MP_JumpBack || stateType_ == CammyState::MP_JumpFront || stateType_ == CammyState::HP_Jump ||
		stateType_ == CammyState::HP_JumpBack || stateType_ == CammyState::HP_JumpFront || stateType_ == CammyState::LK_Jump || stateType_ == CammyState::LK_JumpBack || stateType_ == CammyState::LK_JumpFront ||
		stateType_ == CammyState::MK_Jump || stateType_ == CammyState::MK_JumpBack || stateType_ == CammyState::MK_JumpFront || stateType_ == CammyState::HK_Jump || stateType_ == CammyState::HK_JumpBack || stateType_ == CammyState::HK_JumpFront)
	{
		if (HITSTYLE::Burn == _hitStyle)
		{
			return ChangeState(CammyState::Burn);
		}
		return ChangeState(CammyState::HitWhenJump);
	}
	// 대미지 크기와 유형에 따른 모션 변경?
	switch (_hitStyle)
	{
	case CharacterHandle::HITSTYLE::HITUPPERLIGHT:
		return ChangeState(CammyState::HitUpperLight);
		break;
	case CharacterHandle::HITSTYLE::HITUPPERHEAVY:
		return ChangeState(CammyState::HitUpperHeavy);
		break;
	case CharacterHandle::HITSTYLE::DOWNLIGHT:
		return ChangeState(CammyState::HitDownLight);
		break;
	case CharacterHandle::HITSTYLE::DOWNHEAVY:
		return ChangeState(CammyState::HitDownHeavy);
		break;
	case CharacterHandle::HITSTYLE::CROUCH:
		return ChangeState(CammyState::HitWhenCrouch);
		break;
	case CharacterHandle::HITSTYLE::JUMP:
		return ChangeState(CammyState::HitWhenJump);
		break;
	case CharacterHandle::HITSTYLE::DOWN:
		return ChangeState(CammyState::HitWhenJump);
		break;
	case CharacterHandle::HITSTYLE::BeThrownDown:
		return ChangeState(CammyState::BeThrownDown);
		break;
	case CharacterHandle::HITSTYLE::Burn:
		return ChangeState(CammyState::Burn);
	case CharacterHandle::HITSTYLE::MAX:
		break;
	default:
		break;
	}
}



void Cammy::HitUpperHeavy()
{

	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;

			if (GetModifiedSpriteNumber(166) < mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(166) < mainSpriteRender_->GetCurAnimationFrame())
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
			return ChangeState(CammyState::Idle);
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(166) < mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(166) < mainSpriteRender_->GetCurAnimationFrame())
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
			return ChangeState(CammyState::Idle);
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void Cammy::HitUpperLight()
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
			ChangeState(CammyState::Idle);
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
			ChangeState(CammyState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Cammy::HitDownHeavy()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			if (GetModifiedSpriteNumber(171) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(171) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(CammyState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(171) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(171) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(CammyState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Cammy::HitDownLight()
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
			ChangeState(CammyState::Idle);
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
			ChangeState(CammyState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Cammy::HitWhenCrouch()
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
			ChangeState(CammyState::Idle);
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
			ChangeState(CammyState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Cammy::HitWhenJump()
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
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(CammyState::HitWhenJumpDown);
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
			return ChangeState(CammyState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Cammy::HitWhenJumpDown()
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
			return ChangeState(CammyState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f });
			isJump_ = false;
			return ChangeState(CammyState::GetUp);
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
			return ChangeState(CammyState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f }); // 오차가 나올 때를 대비해 강제로 바닥 y 값에 맞춰 줍니다.
			isJump_ = false;
			return ChangeState(CammyState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void Cammy::Down()
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

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd() || 450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetPos({ GetPos().x, 450.0f });
			return ChangeState(CammyState::GetUp);
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
			ChangeState(CammyState::GetUp);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}


void Cammy::Burn()
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
			return ChangeState(CammyState::HitWhenJumpDown);
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
			return ChangeState(CammyState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Cammy::GetUp()
{
	RemoveHitBox();
	RemoveBodyBox();
	if (GetModifiedSpriteNumber(189) == mainSpriteRender_->GetCurAnimationFrame())
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			SetReversalText();
			return ChangeState(CammyState::CannonSpike);
		}
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::StandDelay);
	}
}


void Cammy::Recovery()
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
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::KO()
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

void Cammy::JumpKO()
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
			return ChangeState(CammyState::KO);
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
			return ChangeState(CammyState::KO);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void Cammy::Win()
{
	if ("P1" == GetName() && 1 <= FightScene::p1WinPoint_)
	{
		ChangeState(CammyState::MatchPointWin);
	}
	if ("P2" == GetName() && 1 <= FightScene::p2WinPoint_)
	{
		ChangeState(CammyState::MatchPointWin);
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}

void Cammy::MatchPointWin()
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}


void Cammy::StandDelay() // 4프레임의 기상 디스어드밴티지 -> 이 행동 동안은 가드밖에 할 수 없다.
{
	RemoveBodyBox();
	RemoveHitBox();
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(CammyState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{
		// 파동권이 존재한다는 것이니까.
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(CammyState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(CammyState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(CammyState::Guard);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::LandDelay() // 점프 후 착지 시 딜레이
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(CammyState::Idle);
	}
}

void Cammy::Guard()
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

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		ResetGuardBox();
		return ChangeState(CammyState::CrouchGuard);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		ResetGuardBox();
		return ChangeState(CammyState::JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
	{
		ResetGuardBox();
		return ChangeState(CammyState::SpinBackKnuckle);
	}


	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Right"))
	{
		return ChangeState(CammyState::Idle);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Left"))
	{

		return ChangeState(CammyState::Idle);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR")
		/* && false == otherCharacter_->GetProjectile()->IsOn() */)
	{
		ResetGuardBox();
		return ChangeState(CammyState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL")
		/* && false == otherCharacter_->GetProjectile()->IsOn() */)
	{
		ResetGuardBox();
		return ChangeState(CammyState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
	{
		ResetGuardBox();
		return ChangeState(CammyState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		ResetGuardBox();
		return ChangeState(CammyState::Walk);
	}

}

void Cammy::CrouchGuard()
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

		return ChangeState(CammyState::Guard);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR"))
	{
		return ChangeState(CammyState::Crouch);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL"))
	{
		return ChangeState(CammyState::Crouch);
	}

}

void Cammy::UpdateAfter()
{
	if (0 >= otherCharacter_->GetCurHp())
	{

		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			if (true == mainSpriteRender_->IsCurSpriteAnimationName("Idle" + GetDirString()))
			{
				return ChangeState(CammyState::R1Win);
			}
			else if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				return ChangeState(CammyState::R1Win);
			}

		}
	}
}


void Cammy::Collision()
{


}