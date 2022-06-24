#include "ChunLi.h"
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
#include <GameCollision.h>
#include "FightScene.h"
#include "CharacterHandle.h"
#include "FightEffects.h"

// Static Var
// Static Func

// constructer destructer
ChunLi::ChunLi()
	: stateType_(ChunLiState::MAX), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr), chargingTimeBack_(0.0f), chargingTimeDown_(0.0f)
{
	hp_ = 170;
	originalHp_ = 170;
	maxJumpHeight_ = 220.0f;
	moveSpeed_ = 400.0f;
	SetRenderOrder(10);
	jumpForce_ = 1300.0f;
}

ChunLi::~ChunLi()
{
}

ChunLi::ChunLi(ChunLi&& _other) noexcept
	: stateType_(ChunLiState::MAX), isDebugOn_(false), isJumpUp_(false), linkedHandle_(nullptr), chargingTimeBack_(0.0f), chargingTimeDown_(0.0f)
{
}

void ChunLi::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();

	if (true == isDebugOn_)
	{
		// *** 하기는 판정박스 디버그용 랜더링입니다.
		//bodyBox_->StartDebugRender();
		collisionBox_->StartDebugRender();
		//hitBoxHead_->StartDebugRender();
		//hitBoxTorso_->StartDebugRender();
		//hitBoxLeg_->StartDebugRender();
		//guardRangeBox_->StartDebugRender();
		//guardBox_->StartDebugRender();
		//crouchGuardBox_->StartDebugRender();
		//strikeBox_->StartDebugRender();
		//strikeBoxOverHead_->StartDebugRender();
		//strikeBoxLow_->StartDebugRender();
		// ->StartDebugRender();
	}
}

void ChunLi::ToggleDebug()
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

void ChunLi::KeyInit()
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

void ChunLi::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("SpritesChunLi.bmp");

	mainSpriteRender_->CreateSpriteAnimation("IdleL", "SpritesChunLi.bmp", 0, 3, true, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("IdleR", "SpritesChunLi.bmp", 0 + 210, 3 + 210, true, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("WalkL", "SpritesChunLi.bmp", 4, 11, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("WalkR", "SpritesChunLi.bmp", 4 + 210, 11 + 210, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("CrouchL", "SpritesChunLi.bmp", 24, 24, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchR", "SpritesChunLi.bmp", 24 + 210, 24 + 210, false, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("GuardL", "SpritesChunLi.bmp", 41, 41, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardL", "SpritesChunLi.bmp", 42, 42, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("GuardR", "SpritesChunLi.bmp", 41 + 210, 41 + 210, true, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("CrouchGuardR", "SpritesChunLi.bmp", 42 + 210, 42 + 210, true, 0.1f);

	// 수직점프 애니메이션 설정
	mainSpriteRender_->CreateSpriteAnimation("JumpL", "SpritesChunLi.bmp", 12, 15, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("JumpR", "SpritesChunLi.bmp", 12 + 210, 15 + 210, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("JumpL", 12, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpL", 15, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpR", 12 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpR", 15 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("JumpFrontL", "SpritesChunLi.bmp", 16, 22, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpFrontR", "SpritesChunLi.bmp", 16 + 210, 22 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 16, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 17, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 21, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontL", 22, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 16 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 17 + 210, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 21 + 210, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpFrontR", 22 + 210, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("JumpBackL", "SpritesChunLi.bmp", 16, 23, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("JumpBackR", "SpritesChunLi.bmp", 16 + 210, 23 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 16, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 17, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 21, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackL", 23, 0.204f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 16 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 17 + 210, 0.255f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 21 + 210, 0.085f);
	mainSpriteRender_->SetChangeFrame("JumpBackR", 23 + 210, 0.204f);

	// 피격 & 다운 & 리커버리 & K.O. & 승리포즈
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightL", "SpritesChunLi.bmp", 158, 158, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperLightR", "SpritesChunLi.bmp", 158 + 210, 158 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyL", "SpritesChunLi.bmp", 158, 159, false, 0.125f);
	mainSpriteRender_->CreateSpriteAnimation("HitUpperHeavyR", "SpritesChunLi.bmp", 158 + 210, 159 + 210, false, 0.125f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownLightL", "SpritesChunLi.bmp", 156, 156, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownLightR", "SpritesChunLi.bmp", 156 + 210, 156 + 210, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyL", "SpritesChunLi.bmp", 156, 157, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HitDownHeavyR", "SpritesChunLi.bmp", 156 + 210, 157 + 210, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchL", "SpritesChunLi.bmp", 160, 160, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenCrouchR", "SpritesChunLi.bmp", 160 + 210, 160 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpL", "SpritesChunLi.bmp", 169, 169, false, 0.408f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpR", "SpritesChunLi.bmp", 169 + 210, 169 + 210, false, 0.408f);

	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownL", "SpritesChunLi.bmp", 163, 164, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("HitWhenJumpDownR", "SpritesChunLi.bmp", 163 + 210, 164 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownL", "SpritesChunLi.bmp", 163, 164, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownDownR", "SpritesChunLi.bmp", 163 + 210, 164 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BeThrownL", "SpritesChunLi.bmp", 198, 201, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("BeThrownR", "SpritesChunLi.bmp", 198 + 210, 201 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("DownL", "SpritesChunLi.bmp", 169, 172, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("DownR", "SpritesChunLi.bmp", 169 + 210, 172 + 210, false, 0.204f);

	mainSpriteRender_->CreateSpriteAnimation("BurnL", "SpritesChunLi.bmp", 195, 197, true, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("BurnR", "SpritesChunLi.bmp", 195 + 210, 197 + 210, true, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("RecoveryL", "SpritesChunLi.bmp", 147 + 210, 151 +210, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("RecoveryR", "SpritesChunLi.bmp", 147, 151, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("GetUpL", "SpritesChunLi.bmp", 164, 166, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("GetUpR", "SpritesChunLi.bmp", 164 + 210, 166 + 210, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 164, 0.306f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 164 + 210, 0.306f);
	mainSpriteRender_->SetChangeFrame("GetUpL", 166, 0.252f);
	mainSpriteRender_->SetChangeFrame("GetUpR", 166 + 210, 0.252f);

	mainSpriteRender_->CreateSpriteAnimation("KOL", "SpritesChunLi.bmp", 169, 172, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("KOR", "SpritesChunLi.bmp", 169 + 210, 172 + 210, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("KOL", 172, 0.85f);
	mainSpriteRender_->SetChangeFrame("KOR", 172 + 210, 0.85f);

	mainSpriteRender_->CreateSpriteAnimation("R1WinL", "SpritesChunLi.bmp", 173, 177, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("R1WinR", "SpritesChunLi.bmp", 173 + 210, 177 + 210, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("R1WinL", 176, 0.85f);
	mainSpriteRender_->SetChangeFrame("R1WinR", 176 + 210, 0.85f);

	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinL", "SpritesChunLi.bmp", 178, 194, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("MatchPointWinR", "SpritesChunLi.bmp", 178 + 210, 194 + 210, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("StandDelayL", "SpritesChunLi.bmp", 12, 12, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("StandDelayR", "SpritesChunLi.bmp", 12 + 210, 12 + 210, false, 0.085f); // 5프레임 지연

	mainSpriteRender_->CreateSpriteAnimation("LandDelayL", "SpritesChunLi.bmp", 12, 12, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LandDelayR", "SpritesChunLi.bmp", 12 + 210, 12 + 210, false, 0.034f); // 2프레임 지연

	// 기본기
	mainSpriteRender_->CreateSpriteAnimation("LPL", "SpritesChunLi.bmp", 26, 28, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("LPR", "SpritesChunLi.bmp", 26 + 210, 28 + 210, false, 0.05f);
	mainSpriteRender_->SetChangeFrame("LPL", 26, 0.051f);
	mainSpriteRender_->SetChangeFrame("LPR", 26 + 210, 0.051f);

	mainSpriteRender_->CreateSpriteAnimation("LP_CloseL", "SpritesChunLi.bmp", 35, 37, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CloseR", "SpritesChunLi.bmp", 35 + 210, 37 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 35, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseL", 37, 0.085f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 35 + 210, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CloseR", 37 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LP_JumpL", "SpritesChunLi.bmp", 78, 79, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_JumpR", "SpritesChunLi.bmp", 78 + 210, 79 + 210, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalL", "SpritesChunLi.bmp", 78, 79, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("LP_DiagonalR", "SpritesChunLi.bmp", 78 + 210, 79 + 210, false, 0.034f);

	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchL", "SpritesChunLi.bmp", 65, 67, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LP_CrouchR", "SpritesChunLi.bmp", 65 + 210, 67 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchL", 66, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchL", 67, 0.085f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchR", 66 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("LP_CrouchR", 67 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("MPL", "SpritesChunLi.bmp", 29, 31, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MPR", "SpritesChunLi.bmp", 29 + 210, 31 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MPL", 31, 0.102f);
	mainSpriteRender_->SetChangeFrame("MPR", 31 + 210, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("MP_JumpL", "SpritesChunLi.bmp", 77, 81, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_JumpR", "SpritesChunLi.bmp", 77 + 210, 81 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_JumpL", 79, 0.3f);
	mainSpriteRender_->SetChangeFrame("MP_JumpR", 79 + 210, 0.3f);

	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalL", "SpritesChunLi.bmp", 82, 84, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_DiagonalR", "SpritesChunLi.bmp", 82 + 210, 84 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_DiagonalL", 83, 0.3f);
	mainSpriteRender_->SetChangeFrame("MP_DiagonalR", 83 + 210, 0.3f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CloseL", "SpritesChunLi.bmp", 42, 44, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CloseR", "SpritesChunLi.bmp", 42 + 210, 44 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CloseL", 43, 0.153f);
	mainSpriteRender_->SetChangeFrame("MP_CloseL", 44, 0.170f);
	mainSpriteRender_->SetChangeFrame("MP_CloseR", 43 + 210, 0.153f);
	mainSpriteRender_->SetChangeFrame("MP_CloseR", 44 + 210, 0.170f);

	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchL", "SpritesChunLi.bmp", 65, 67, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MP_CrouchR", "SpritesChunLi.bmp", 65 + 210, 67 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 66, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchL", 67, 0.085f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 66 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("MP_CrouchR", 67 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HPL", "SpritesChunLi.bmp", 32, 34, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HPR", "SpritesChunLi.bmp", 32 + 210, 34 + 210, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("HPL", 34, 0.374f);
	mainSpriteRender_->SetChangeFrame("HPR", 34 + 210, 0.374f);

	mainSpriteRender_->CreateSpriteAnimation("HP_JumpL", "SpritesChunLi.bmp",77, 81, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_JumpR", "SpritesChunLi.bmp", 77 + 210, 81 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_JumpL", 79, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_JumpR", 79 + 210, 0.34f);

	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalL", "SpritesChunLi.bmp", 82, 84, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_DiagonalR", "SpritesChunLi.bmp", 82 + 210, 84 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalL", 83, 0.34f);
	mainSpriteRender_->SetChangeFrame("HP_DiagonalR", 83 + 210, 0.34f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CloseL", "SpritesChunLi.bmp", 38, 40, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CloseR", "SpritesChunLi.bmp", 38 + 210, 40 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 38, 0.034f);
	mainSpriteRender_->SetChangeFrame("HP_CloseL", 39, 0.102f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 38 + 210, 0.034f);
	mainSpriteRender_->SetChangeFrame("HP_CloseR", 39 + 210, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchL", "SpritesChunLi.bmp", 65, 67, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HP_CrouchR", "SpritesChunLi.bmp", 65 + 210, 67 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 66, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchL", 67, 0.170f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 66 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HP_CrouchR", 67 + 210, 0.170f);

	mainSpriteRender_->CreateSpriteAnimation("LKL", "SpritesChunLi.bmp", 44, 46, false, 0.066f);
	mainSpriteRender_->CreateSpriteAnimation("LKR", "SpritesChunLi.bmp", 44 + 210, 46 + 210, false, 0.066f);
	mainSpriteRender_->SetChangeFrame("LKL", 46, 0.085f);
	mainSpriteRender_->SetChangeFrame("LKR", 46 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_JumpL", "SpritesChunLi.bmp", 85, 87, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_JumpR", "SpritesChunLi.bmp", 85 + 210, 87 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_JumpL", 86, 0.68f);
	mainSpriteRender_->SetChangeFrame("LK_JumpR", 86 + 210, 0.68f);

	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalL", "SpritesChunLi.bmp", 97, 97, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("LK_DiagonalR", "SpritesChunLi.bmp", 97 + 210, 97 + 210, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CloseL", "SpritesChunLi.bmp", 44, 45, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CloseR", "SpritesChunLi.bmp", 44 + 210, 45 + 210, false, 0.102f);
	mainSpriteRender_->SetChangeFrame("LK_CloseL", 45, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CloseR", 45 + 210, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchL", "SpritesChunLi.bmp", 68, 70, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("LK_CrouchR", "SpritesChunLi.bmp", 68 + 210, 70 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 69, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchL", 70, 0.085f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 70 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("LK_CrouchR", 69 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("MKL", "SpritesChunLi.bmp", 44, 49, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("MKR", "SpritesChunLi.bmp", 44 + 210, 49 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("MKL", 49, 0.119f);
	mainSpriteRender_->SetChangeFrame("MKR", 49 + 210, 0.119f);

	mainSpriteRender_->CreateSpriteAnimation("MK_JumpL", "SpritesChunLi.bmp", 85, 89, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_JumpR", "SpritesChunLi.bmp", 85 + 210, 89 + 210, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_JumpL", 87, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_JumpR", 87 + 210, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalL", "SpritesChunLi.bmp", 96, 98, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("MK_DiagonalR", "SpritesChunLi.bmp", 96 + 210, 98 + 210, false, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalL", 97, 0.221f);
	mainSpriteRender_->SetChangeFrame("MK_DiagonalR", 97 + 210, 0.221f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CloseL", "SpritesChunLi.bmp", 54, 55, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CloseR", "SpritesChunLi.bmp", 54 + 210, 55 + 210, false, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CloseL", 55, 0.102f);
	mainSpriteRender_->SetChangeFrame("MK_CloseR", 55 + 210, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchL", "SpritesChunLi.bmp", 71, 73, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("MK_CrouchR", "SpritesChunLi.bmp", 71 + 210, 73 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 72, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchL", 73, 0.085f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 72 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("MK_CrouchR", 73 + 210, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HKL", "SpritesChunLi.bmp", 50, 53, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HKR", "SpritesChunLi.bmp", 50 + 210, 53 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("HKL", 51, 0.068f);
	mainSpriteRender_->SetChangeFrame("HKL", 52, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKL", 53, 0.153f);
	mainSpriteRender_->SetChangeFrame("HKR", 51 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HKR", 52 + 210, 0.136f);
	mainSpriteRender_->SetChangeFrame("HKR", 53 + 210, 0.153f);

	mainSpriteRender_->CreateSpriteAnimation("HK_JumpL", "SpritesChunLi.bmp", 55, 59, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_JumpR", "SpritesChunLi.bmp", 55 + 210, 59 + 210, false, 0.034f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 57, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 58, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpL", 59, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 57 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 58 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_JumpR", 59 + 210, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalL", "SpritesChunLi.bmp", 99, 104, false, 0.085f);
	mainSpriteRender_->CreateSpriteAnimation("HK_DiagonalR", "SpritesChunLi.bmp", 99 + 210, 104 + 210, false, 0.085f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CloseL", "SpritesChunLi.bmp", 124, 129, false, 0.136f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CloseR", "SpritesChunLi.bmp", 124 + 210, 129 + 210, false, 0.136f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 128, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseL", 129, 0.187f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 128 + 210, 0.068f);
	mainSpriteRender_->SetChangeFrame("HK_CloseR", 129 + 210, 0.187f);

	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchL", "SpritesChunLi.bmp", 74, 76, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HK_CrouchR", "SpritesChunLi.bmp", 74 + 210, 76 + 210, false, 0.034f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 75, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchL", 76, 0.182f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 75 + 210, 0.153f);
	mainSpriteRender_->SetChangeFrame("HK_CrouchR", 76 + 210, 0.182f);

	// 필살기
	// 중 기공권
	mainSpriteRender_->CreateSpriteAnimation("KikokenL", "SpritesChunLi.bmp", 108, 111, false, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("KikokenR", "SpritesChunLi.bmp", 108 + 210, 111 + 210, false, 0.051f);
	mainSpriteRender_->SetChangeFrame("KikokenL", 111, 0.331f);
	mainSpriteRender_->SetChangeFrame("KikokenR", 111 + 210, 0.331f);

	// 강 스피닝 버드 킥
	mainSpriteRender_->CreateSpriteAnimation("SpinBirdKickL", "SpritesChunLi.bmp", 136, 151, false, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("SpinBirdKickR", "SpritesChunLi.bmp", 136 + 210, 151 + 210, false, 0.068f);

	// 약 백열각
	mainSpriteRender_->CreateSpriteAnimation("HyakuretsuKyakuL", "SpritesChunLi.bmp", 117, 123, true, 0.068f);
	mainSpriteRender_->CreateSpriteAnimation("HyakuretsuKyakuR", "SpritesChunLi.bmp", 117 + 210, 123 + 210, true, 0.068f);

	mainSpriteRender_->CreateSpriteAnimation("ThrowL", "SpritesChunLi.bmp", 152 + 210, 155 + 210, false, 0.204f);
	mainSpriteRender_->CreateSpriteAnimation("ThrowR", "SpritesChunLi.bmp", 152, 155, false, 0.204f);
	mainSpriteRender_->SetChangeFrame("ThrowL", 152 + 210, 0.306f);
	mainSpriteRender_->SetChangeFrame("ThrowR", 152, 0.306f);
	mainSpriteRender_->SetChangeFrame("ThrowL", 153 + 210, 0.102f);
	mainSpriteRender_->SetChangeFrame("ThrowR", 153, 0.102f);
}

void ChunLi::LastCheck()
{
	AnimationInit();
	dirstate_ = ACTORDIR::LOOK_LEFT;
	ChangeState(ChunLiState::Idle);
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	soundPlayer_->PlayCountReset(1);
	voicePlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	voicePlayer_->PlayCountReset(1);
}

void ChunLi::ChangeState(ChunLiState _stateType)
{
	if (_stateType == ChunLiState::NONE)
	{
		return;
	}
	stateType_ = _stateType;

	switch (stateType_)
	{
	case ChunLiState::Idle:
		StateName = "Idle";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Walk:
		StateName = "Walk";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Crouch:
		StateName = "Crouch";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Jump:
		StateName = "Jump";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::JumpFront:
		StateName = "JumpFront";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::JumpBack:
		StateName = "JumpBack";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Guard:
		StateName = "Guard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::CrouchGuard:
		StateName = "CrouchGuard";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HitUpperLight:
		StateName = "HitUpperLight";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HitUpperHeavy:
		StateName = "HitUpperHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HitDownLight:
		StateName = "HitDownLight";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HitDownHeavy:
		StateName = "HitDownHeavy";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HitWhenCrouch:
		StateName = "HitWhenCrouch";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HitWhenJump:
		StateName = "HitWhenJump";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HitWhenJumpDown:
		StateName = "HitWhenJumpDown";
		JumpDir = float4::DOWN * jumpForce_;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Down:
		StateName = "Down";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Recovery:
		StateName = "Recovery";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::GetUp:
		StateName = "GetUp";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::KO:
		StateName = "KO";
		JumpDir = float4::UP * jumpForce_ * 0.5f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::JumpKO:
		StateName = "HitWhenJump";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::R1Win:
		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			StateName = "R1Win";
			mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		}
		break;
	case ChunLiState::MatchPointWin:
		StateName = "MatchPointWin";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::StandDelay:
		StateName = "StandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::LandDelay:
		StateName = "LandDelay";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LP:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::LP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::LP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LP_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "LP_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::MP:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "MP_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::MP_JumpBack:

		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MP_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::HP:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::HP_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::HP_Crouch:
		strikeBox_->SetCollisionCount(1);
		StateName = "HP_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::HP_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::HP_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HP_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HP_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LK:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "LK_Close";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "LK_Crouch";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Jump";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::LK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::LK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "LK_Diagonal";
		soundPlayer_->PlayOverlap("LAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MK:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MK_Close:
		strikeBox_->SetCollisionCount(1);
		StateName = "MK_Close";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "MK_Crouch";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::MK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Jump";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::MK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::MK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "MK_Diagonal";
		soundPlayer_->PlayOverlap("MAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HK:
		strikeBox_->SetCollisionCount(1);
		StateName = "HK";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HK_Close:
		strikeBox_->SetCollisionCount(2);
		StateName = "HK_Close";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HK_Crouch:
		strikeBoxLow_->SetCollisionCount(1);
		StateName = "HK_Crouch";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;

	case ChunLiState::HK_Jump:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Jump";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HK_JumpFront:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::HK_JumpBack:
		strikeBoxOverHead_->SetCollisionCount(1);
		StateName = "HK_Diagonal";
		soundPlayer_->PlayOverlap("HAttack.wav", 0);
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Kikoken:
		if (nullptr != projectile_)
		{
			break;
		}
		isShooting_ = false;
		strikeBox_->SetCollisionCount(0);
		StateName = "Kikoken";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::SpinBirdKick:
		strikeBox_->SetCollisionCount(1);
		StateName = "SpinBirdKick";
		JumpDir = float4::UP * jumpForce_;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("ChunLi_SpinBirdKick.mp3", 0);
		break;
	case ChunLiState::HyakuretsuKyaku:
		strikeBox_->SetCollisionCount(1);
		StateName = "HyakuretsuKyaku";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Throw:
		StateName = "Throw";
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		voicePlayer_->PlayOverlap("ChunLi_Throw.mp3", 0);
		break;
	case ChunLiState::BeThrown:
		StateName = "BeThrown";
		JumpDir = float4::UP * jumpForce_ * 0.1f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::BeThrownDown:
		StateName = "BeThrownDown";
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	case ChunLiState::Burn:
		StateName = "Burn";
		JumpDir = float4::UP * jumpForce_ * 0.7f;
		knockBackDir_ = 300.0f;
		mainSpriteRender_->ChangeSpriteAnimation(StateName + GetDirString());
		break;
	default:
		break;
	}
}

void ChunLi::UpdateBefore()
{
	ToggleDebug();

	if (this->hp_ <= 0)
	{
		voicePlayer_->PlayOverlap("ChunLi_KO.mp3", 0);
		if (true == mainSpriteRender_->IsCurSpriteAnimationName("BeThrownDown" + GetDirString()))
		{
			LeftRightCheck();
		}
		if (450.0f > GetPos().y)
		{
			return ChangeState(ChunLiState::JumpKO);

		}
		return ChangeState(ChunLiState::KO);
	}
}


std::string ChunLi::GetDirString() // 업데이트로 체크된 "지향 방향" 을 측정해 글자를 리턴
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

void ChunLi::LeftRightCheck() // "지향 방향" 을 체크해 스프라이트와 판정박스의 방향을 돌려준다.
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

void ChunLi::Update()
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

	switch (dirstate_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
		{
			chargingTimeBack_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		}
		if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
		{
			chargingTimeBack_ = 0.0f;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
		{
			chargingTimeBack_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		}
		if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
		{
			chargingTimeBack_ = 0.0f;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		chargingTimeDown_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
	}
	if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		chargingTimeDown_ = 0.0f;
	}

	if (true == IsOn())
	{
		switch (stateType_)
		{
		case ChunLiState::Idle:
			Idle();
			break;
		case ChunLiState::Walk:
			Walk();
			break;
		case ChunLiState::Crouch:
			Crouch();
			break;
		case ChunLiState::Jump:
			Jump();
			break;
		case ChunLiState::JumpFront:
			JumpFront();
			break;
		case ChunLiState::JumpBack:
			JumpBack();
			break;
		case ChunLiState::Guard:
			Guard();
			break;
		case ChunLiState::CrouchGuard:
			CrouchGuard();
			break;
		case ChunLiState::HitUpperLight:
			HitUpperLight();
			break;
		case ChunLiState::HitUpperHeavy:
			HitUpperHeavy();
			break;
		case ChunLiState::HitDownLight:
			HitDownLight();
			break;
		case ChunLiState::HitDownHeavy:
			HitDownHeavy();
			break;
		case ChunLiState::HitWhenCrouch:
			HitWhenCrouch();
			break;
		case ChunLiState::HitWhenJump:
			HitWhenJump();
			break;
		case ChunLiState::HitWhenJumpDown:
			HitWhenJumpDown();
			break;
		case ChunLiState::Down:
			Down();
			break;
		case ChunLiState::Recovery:
			Recovery();
			break;
		case ChunLiState::GetUp:
			GetUp();
			break;
		case ChunLiState::KO:
			KO();
			break;
		case ChunLiState::JumpKO:
			JumpKO();
			break;
		case ChunLiState::R1Win:
			Win();
			break;
		case ChunLiState::MatchPointWin:
			MatchPointWin();
			break;
		case ChunLiState::StandDelay:
			StandDelay();
			break;
		case ChunLiState::LandDelay:
			LandDelay();
			break;
		case ChunLiState::LP:
			LP();
			break;
		case ChunLiState::LP_Close:
			LP_Close();
			break;
		case ChunLiState::LP_Crouch:
			LP_Crouch();
			break;
		case ChunLiState::LP_Jump:
			LP_Jump();
			break;
		case ChunLiState::LP_JumpFront:
			LP_JumpFront();
			break;
		case ChunLiState::LP_JumpBack:
			LP_JumpBack();
			break;
		case ChunLiState::MP:
			MP();
			break;
		case ChunLiState::MP_Close:
			MP_Close();
			break;
		case ChunLiState::MP_Crouch:
			MP_Crouch();
			break;
		case ChunLiState::MP_Jump:
			MP_Jump();
			break;
		case ChunLiState::MP_JumpFront:
			MP_JumpFront();
			break;
		case ChunLiState::MP_JumpBack:
			MP_JumpBack();
			break;
		case ChunLiState::HP:
			HP();
			break;
		case ChunLiState::HP_Close:
			HP_Close();
			break;
		case ChunLiState::HP_Crouch:
			HP_Crouch();
			break;
		case ChunLiState::HP_Jump:
			HP_Jump();
			break;
		case ChunLiState::HP_JumpFront:
			HP_JumpFront();
			break;
		case ChunLiState::HP_JumpBack:
			HP_JumpBack();
			break;
		case ChunLiState::LK:
			LK();
			break;
		case ChunLiState::LK_Close:
			LK_Close();
			break;
		case ChunLiState::LK_Crouch:
			LK_Crouch();
			break;
		case ChunLiState::LK_Jump:
			LK_Jump();
			break;
		case ChunLiState::LK_JumpFront:
			LK_JumpFront();
			break;
		case ChunLiState::LK_JumpBack:
			LK_JumpBack();
			break;
		case ChunLiState::MK:
			MK();
			break;
		case ChunLiState::MK_Close:
			MK_Close();
			break;
		case ChunLiState::MK_Crouch:
			MK_Crouch();
			break;
		case ChunLiState::MK_Jump:
			MK_Jump();
			break;
		case ChunLiState::MK_JumpFront:
			MK_JumpFront();
			break;
		case ChunLiState::MK_JumpBack:
			MK_JumpBack();
			break;
		case ChunLiState::HK:
			HK();
			break;
		case ChunLiState::HK_Close:
			HK_Close();
			break;
		case ChunLiState::HK_Crouch:
			HK_Crouch();
			break;
		case ChunLiState::HK_Jump:
			HK_Jump();
			break;
		case ChunLiState::HK_JumpFront:
			HK_JumpFront();
			break;
		case ChunLiState::HK_JumpBack:
			HK_JumpBack();
			break;
		case ChunLiState::Kikoken:
			Kikoken();
			break;
		case ChunLiState::SpinBirdKick:
			SpinBirdKick();
			break;
		case ChunLiState::HyakuretsuKyaku:
			HyakuretsuKyaku();
			break;
		case ChunLiState::Throw:
			Throw();
			break;
		case ChunLiState::BeThrown:
			BeThrown();
			break;
		case ChunLiState::BeThrownDown:
			BeThrownDown();
			break;
		case ChunLiState::Burn:
			Burn();
			break;
		default:
			break;
		}
	}
}


void ChunLi::Idle()
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
		return ChangeState(ChunLiState::Walk);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(ChunLiState::Crouch);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		return ChangeState(ChunLiState::Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::LP_Close);
		}
		else
		{
			return ChangeState(ChunLiState::LP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::MP_Close);
		}
		else
		{
			return ChangeState(ChunLiState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::HP_Close);
		}
		else
		{
			return ChangeState(ChunLiState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::LK_Close);
		}
		else
		{
			return ChangeState(ChunLiState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::MK_Close);
		}
		else
		{
			return ChangeState(ChunLiState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::HK_Close);
		}
		else
		{
			return ChangeState(ChunLiState::HK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
	{
		return ChangeState(ChunLiState::HyakuretsuKyaku);
	}
}


void ChunLi::Walk()
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
			return ChangeState(ChunLiState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{
		// 파동권이 존재한다는 것이니까.
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(ChunLiState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
			|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(ChunLiState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(ChunLiState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP") && nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{ // 잡기 시동
			switch (dirstate_)
			{
			case CharacterHandle::ACTORDIR::LOOK_RIGHT:
				chargingTimeBack_ = 0.0f;
				return ChangeState(ChunLiState::Throw);
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
				chargingTimeBack_ = 0.0f;
				return ChangeState(ChunLiState::JumpFront);
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(ChunLiState::JumpBack);
				break;
			default:
				break;
			}
		}
		if (chargingTimeBack_ >= 1.0f && true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			switch (dirstate_)
			{
			case ACTORDIR::LOOK_RIGHT: 
				break;
			case ACTORDIR::LOOK_LEFT:
				return ChangeState(ChunLiState::Kikoken);
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
				chargingTimeBack_ = 0.0f;
				return ChangeState(ChunLiState::Throw);
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
				return ChangeState(ChunLiState::JumpBack);
				break;
			case ACTORDIR::LOOK_LEFT:
				chargingTimeBack_ = 0.0f;
				return ChangeState(ChunLiState::JumpFront);
				break;
			default:
				break;
			}
		}

		if (chargingTimeBack_ >= 1.0f && true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			switch (dirstate_)
			{
			case ACTORDIR::LOOK_RIGHT: // 오른 쪽을 바라보고 있으며, 왼쪽 화살표키 + 필살기키를 누르면
				return ChangeState(ChunLiState::Kikoken);
				break;
			case ACTORDIR::LOOK_LEFT:
				break;
			default:
				break;
			}
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(ChunLiState::Crouch);
	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::LP_Close);
		}
		else
		{
			return ChangeState(ChunLiState::LP);
		}

	}

	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::MP_Close);
		}
		else
		{
			return ChangeState(ChunLiState::MP);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::HP_Close);
		}
		else
		{
			return ChangeState(ChunLiState::HP);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::LK_Close);
		}
		else
		{
			return ChangeState(ChunLiState::LK);
		}
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::MK_Close);
		}
		else
		{
			return ChangeState(ChunLiState::MK);
		}

	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		if (nullptr != bodyBox_->CollisionGroupCheckSingle(enemyBody_))
		{
			return ChangeState(ChunLiState::HK_Close);
		}
		else
		{
			return ChangeState(ChunLiState::HK);
		}
	}

	if (
		false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right")
		&& false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left")
		)
	{
		return ChangeState(ChunLiState::Idle); // 꼭 return 으로 해주기.. 왜? if문 밖에 또 ChangeState() 있을 수 있으니...
	}
}


void ChunLi::Crouch()
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
				return ChangeState(ChunLiState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
			ACTORDIR::LOOK_RIGHT == dirstate_)
		{
			// 파동권이 존재한다는 것이니까.
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(ChunLiState::CrouchGuard);
			}
		}

		if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
		{
			if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
				|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
			{
				return ChangeState(ChunLiState::CrouchGuard);
			}
		}
		else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
			ACTORDIR::LOOK_LEFT == dirstate_)
		{
			if (nullptr != otherCharacter_->projectile_
				&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
			{
				return ChangeState(ChunLiState::CrouchGuard);
			}
		}


		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
		{
			return ChangeState(ChunLiState::LP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
		{
			return ChangeState(ChunLiState::MP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
		{
			return ChangeState(ChunLiState::HP_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
		{
			return ChangeState(ChunLiState::LK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
		{
			return ChangeState(ChunLiState::MK_Crouch);
		}
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
		{
			return ChangeState(ChunLiState::HK_Crouch);
		}
		if (1.0f <= chargingTimeDown_ && true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			return ChangeState(ChunLiState::SpinBirdKick);
		}


		return ChangeState(ChunLiState::Crouch);
	}
	else if (false == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Down"))
	{
		return ChangeState(ChunLiState::Idle);
	}

}

void ChunLi::Jump() // 수직점프
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(ChunLiState::LP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(ChunLiState::MP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(ChunLiState::HP_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(ChunLiState::LK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(ChunLiState::MK_Jump);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(ChunLiState::HK_Jump);
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
			return ChangeState(ChunLiState::LandDelay);
		}
	}
	return;
}


void ChunLi::JumpFront()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	isJump_ = true;
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(ChunLiState::LP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(ChunLiState::MP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(ChunLiState::HP_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(ChunLiState::LK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(ChunLiState::MK_JumpFront);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(ChunLiState::HK_JumpFront);
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
				return ChangeState(ChunLiState::LandDelay);
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
				return ChangeState(ChunLiState::LandDelay);
			}
		}
		return;
		break;
	default:
		break;
	}

}

void ChunLi::JumpBack()
{
	hitBoxTorso_->SetPivot({ 0,-30 });
	hitBoxTorso_->SetSize({ 100, 190 });
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LP"))
	{
		return ChangeState(ChunLiState::LP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MP"))
	{
		return ChangeState(ChunLiState::MP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HP"))
	{
		return ChangeState(ChunLiState::HP_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_LK"))
	{
		return ChangeState(ChunLiState::LK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_MK"))
	{
		return ChangeState(ChunLiState::MK_JumpBack);
	}
	if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_HK"))
	{
		return ChangeState(ChunLiState::HK_JumpBack);
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
				return ChangeState(ChunLiState::LandDelay);
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
				return ChangeState(ChunLiState::LandDelay);
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


void ChunLi::LP()
{
	SetStrikeBox(mainSpriteRender_, 27, 28, 210, { 80,-45 }, { 128,30 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}

}

void ChunLi::LP_Close()
{
	SetStrikeBox(mainSpriteRender_, 36, 37, 210, { 80,-70 }, { 80,35 }, 4, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}


void ChunLi::LP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 66, 67, 210, { 150, 15 }, { 160,35 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Crouch);
	}
}

void ChunLi::LP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 79, 80, 210, { 50, -50 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
	}
}

void ChunLi::LP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 79, 80, 210, { 50, -50 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::LP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 79, 80, 210, { 50, -50 }, { 100,100 }, 20, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void ChunLi::MP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({20,15}) };
	hitBoxTorso_->SetSize({ 120, 210 });

	SetStrikeBox(mainSpriteRender_, 30, 31, 210, { 180, -15 }, { 115,65 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::MP_Close()
{
	SetStrikeBox(mainSpriteRender_, 43, 44, 210, { 100, -30 }, { 130,80 }, 22, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::MP_Crouch()
{
	SetStrikeBox(mainSpriteRender_, 66, 67, 210, { 150, 15 }, { 170,40 }, 22, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Crouch);
	}
}

void ChunLi::MP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 79, 80, 210, { 70, -50 }, { 120,80 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
	}
}

void ChunLi::MP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 83, 84, 210, { 70, -50 }, { 120,80 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::MP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 83, 84, 210, { 70, -50 }, { 120,80 }, 24, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void ChunLi::HP()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,15 }));
	hitBoxTorsoPivot_ = { SetModifiedHitbox({30,15}) };
	hitBoxTorso_->SetSize({ 130, 210 });
	SetStrikeBox(mainSpriteRender_, 33, 34, 210, { 160, -65 }, { 115,75 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}


void ChunLi::HP_Close()
{
	SetStrikeBox(mainSpriteRender_, 39, 40, 210, { 120, -120 }, { 130,100 }, 28, HITSTYLE::HITUPPERHEAVY, "HIGH");

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::HP_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 20,15 }));
	hitBoxTorso_->SetSize({ 100,220 });
	SetStrikeBox(mainSpriteRender_, 66, 67, 210, { 150, 0 }, { 160,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Crouch);
	}
}

void ChunLi::HP_Jump()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 79, 80, 210, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
	}
}

void ChunLi::HP_JumpFront()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 83, 84, 210, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::HP_JumpBack()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -30,-70 }));
	hitBoxTorso_->SetSize({ 90, 170 });
	SetStrikeBoxOverHead(mainSpriteRender_, 83, 84, 210, { 70, -30 }, { 130,50 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::LK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });

	SetStrikeBox(mainSpriteRender_, 45, 46, 210, { 130, 0 }, { 160,85 }, 14, HITSTYLE::HITUPPERLIGHT, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::LK_Close()
{
	SetStrikeBox(mainSpriteRender_, 44, 45, 210, { 120, 50 }, { 160,110 }, 14, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::LK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 69, 70, 210, { 110, 115 }, { 160,55 }, 4, HITSTYLE::DOWNLIGHT, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Crouch);
	}
}

void ChunLi::LK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 86, 87, 210, { 20, -90 }, { 150,100 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
	}
}

void ChunLi::LK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 210, { 70, 0 }, { 110,90 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::LK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 210, { 70, 0 }, { 110,90 }, 12, HITSTYLE::DOWNHEAVY, "MID");
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
				return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void ChunLi::MK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -50,15 }));
	hitBoxTorso_->SetSize({ 140, 220 });
	SetStrikeBox(mainSpriteRender_, 46, 48, 210, { 130, -10 }, { 170,95 }, 24, HITSTYLE::HITUPPERHEAVY, "HIGH");

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::MK_Close()
{
	SetStrikeBox(mainSpriteRender_, 55, 56, 210, { 100, -10 }, { 100,100 }, 24, HITSTYLE::DOWNHEAVY, "MID");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::MK_Crouch()
{
	SetStrikeBoxLow(mainSpriteRender_, 72, 73, 210, { 160, 115 }, { 200,55 }, 22, HITSTYLE::DOWNHEAVY, "LOW");
	if (GetModifiedSpriteNumber(73) == mainSpriteRender_->GetCurAnimationFrame() || GetModifiedSpriteNumber(74) == mainSpriteRender_->GetCurAnimationFrame())
	{
		if (true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
		{
			SetInFightText();
			ChangeState(ChunLiState::HyakuretsuKyaku);
		}
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Crouch);
	}
}

void ChunLi::MK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 87, 88, 210, { 20, -90 }, { 150,100 }, 22, HITSTYLE::DOWNHEAVY, "MID");
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

			return ChangeState(ChunLiState::LandDelay);
		}
		return;
	}
}

void ChunLi::MK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 210, { 80, 20 }, { 170,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
			return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::MK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 97, 98, 210, { 80, 20 }, { 170,55 }, 22, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
				return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}


void ChunLi::HK()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ -10,35 }));
	hitBoxTorso_->SetSize({ 170, 185 });
	SetStrikeBox(mainSpriteRender_, 51, 52, 210, { 120, -80 }, { 140,105 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::HK_Close()
{
	SetStrikeBox(mainSpriteRender_, 126, 129, 210, { 100, -90 }, { 120,190 }, 26, HITSTYLE::DOWN, "HIGH");
	// 애니메이션은 한 번 재생되고, 그 "재생" 시간동안 여기 있어야 한다면...
	if (GetModifiedSpriteNumber(126) == mainSpriteRender_->GetCurAnimationFrame())
	{
		voicePlayer_->PlayOverlap("ChunLi_Hakuretsukyaku.mp3", 0);
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::HK_Crouch()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 30,60 }));
	hitBoxTorso_->SetSize({ 120, 165 });
	SetStrikeBoxLow(mainSpriteRender_, 75, 76, 210, { 155, 115 }, { 110,55 }, 30, HITSTYLE::DOWN, "LOW");
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Crouch);
	}
}

void ChunLi::HK_Jump()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 55, 58, 210, { 100, -20 }, { 140,170 }, 28, HITSTYLE::DOWNHEAVY, "MID");
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
	}
}

void ChunLi::HK_JumpFront()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 101, 102, 210, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	SetStrikeBoxOverHead(mainSpriteRender_, 103, 104, 210, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
			return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::HK_JumpBack()
{
	SetStrikeBoxOverHead(mainSpriteRender_, 101, 102, 210, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
	SetStrikeBoxOverHead(mainSpriteRender_, 103, 104, 210, { 100, 0 }, { 200,55 }, 30, HITSTYLE::HITUPPERHEAVY, "HIGH");
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
				return ChangeState(ChunLiState::LandDelay);
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
			return ChangeState(ChunLiState::LandDelay);
		}
		return;
		break;
	default:
		break;
	}
}

void ChunLi::Kikoken()
{
	hitBoxTorso_->SetPivot(SetModifiedHitbox({ 45,15 }));
	hitBoxTorso_->SetSize({ 200,190 });

	// projectile_ 이 파동권일 것이다.

	if (nullptr == projectile_ && false == isShooting_)
	{
		voicePlayer_->PlayAlone("ChunLi_Kikoken.mp3", 0);
		projectile_ = GetScene()->CreateObject<Projectile>();
		projectile_->ConvertToKikoken();
		projectile_->SetName(GetName() + "Projectile");
		projectile_->SetParentCharacter(this);
		projectile_->SetShotDir();
		projectile_->CollisionInit();
		isShooting_ = true;
	}


if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
{
	return ChangeState(ChunLiState::Idle);
}
}


void ChunLi::SpinBirdKick()
{
	SetStrikeBox(mainSpriteRender_, 144, 145, 210, { 130, -30 }, { 170,95 }, 24, HITSTYLE::DOWN, "HIGH");
	SetStrikeBox(mainSpriteRender_, 146, 147, 210, { 130, -30 }, { 170,95 }, 24, HITSTYLE::DOWN, "HIGH");

	if (GetModifiedSpriteNumber(141) < mainSpriteRender_->GetCurAnimationFrame())
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
		return ChangeState(ChunLiState::LandDelay);
	}
}

void ChunLi::HyakuretsuKyaku()
{
	SetStrikeBox(mainSpriteRender_, 117, 118, 210, { 130, -30 }, { 170,95 }, 24, HITSTYLE::JUMP, "HIGH");
	voicePlayer_->PlayAlone("ChunLi_Hakuretsukyaku.mp3", 2);
	SetStrikeBox(mainSpriteRender_, 119, 120, 210, { 130, 0 }, { 170,95 }, 24, HITSTYLE::JUMP, "HIGH");
	SetStrikeBox(mainSpriteRender_, 121, 122, 210, { 130, -10 }, { 170,95 }, 24, HITSTYLE::JUMP, "HIGH");
	SetStrikeBox(mainSpriteRender_, 123, 124, 210, { 130, 20 }, { 170,95 }, 24, HITSTYLE::JUMP, "HIGH");

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		voicePlayer_->Stop();
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::Throw()
{
	RemoveCollisionBox();
	otherCharacter_->SetHold(33);
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::BeThrown()
{
	RemoveCollisionBox();
	if (GetModifiedSpriteNumber(200) < mainSpriteRender_->GetCurAnimationFrame())
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
			JumpDir += float4::DOWN * 2000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
			return;
		}
		curDirectionY_ = -1;
		JumpDir += float4::DOWN * 2000.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		SetMove(JumpDir * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayCountReset(1);
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			hp_ -= 33;
			return ChangeState(ChunLiState::BeThrownDown);
		}

	}
}


void ChunLi::BeThrownDown()
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
			return ChangeState(ChunLiState::GetUp);
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
			return ChangeState(ChunLiState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void ChunLi::SetHold(int _damage)
{
	ChangeState(ChunLiState::BeThrown);
}

// SetStrikeBox() : 타격 판정 박스를 특정 애니메이션 프레임에 활성화시킨 후 다음 프레임에 없애 주는 함수
// 상단기
void ChunLi::SetStrikeBox(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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
void ChunLi::SetStrikeBoxOverHead(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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
void ChunLi::SetStrikeBoxLow(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
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

void ChunLi::ResetStrikeBox()
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

void ChunLi::ResetGuardBox()
{
	guardBoxPivot_ = { 0, 2000 };
	guardBox_->SetPivot({ 0,2000 });
	guardBox_->SetSize({ 10,10 });
	crouchGuardBoxPivot_ = { 0, 2000 };
	crouchGuardBox_->SetPivot({ 0, 2000 });
	crouchGuardBox_->SetSize({ 10,10 });
}

void ChunLi::ResetHitBox()
{
	//hitBoxHeadPivot_ = { 12, -75 };
	hitBoxTorsoPivot_ = { -10, 15 };
	//hitBoxLegPivot_ = { -8, 84 };
	//hitBoxHead_->SetSize({ 42,39 });
	hitBoxTorso_->SetSize({ 100,210 });
	//hitBoxLeg_->SetSize({ 100,105 });
}

void ChunLi::ResetBodyBox()
{
	bodyBox_->SetPivot({ 0,15 });
	bodyBox_->SetSize({ 120,220 });
}


void ChunLi::SetDamaged(int _damage, HITSTYLE _hitStyle)
{
	hp_ -= _damage; // 체력 차감
	if (stateType_ == ChunLiState::Crouch || stateType_ == ChunLiState::CrouchGuard) // 피격자가 앉아 있을 때는 무조건 앉은 상태의 피격 모션이 사용된다.
	{
		return ChangeState(ChunLiState::HitWhenCrouch);
	}

	if (stateType_ == ChunLiState::Jump || stateType_ == ChunLiState::JumpBack || stateType_ == ChunLiState::JumpFront || stateType_ == ChunLiState::LP_Jump || stateType_ == ChunLiState::LP_JumpBack ||
		stateType_ == ChunLiState::LP_JumpFront || stateType_ == ChunLiState::MP_Jump || stateType_ == ChunLiState::MP_JumpBack || stateType_ == ChunLiState::MP_JumpFront || stateType_ == ChunLiState::HP_Jump ||
		stateType_ == ChunLiState::HP_JumpBack || stateType_ == ChunLiState::HP_JumpFront || stateType_ == ChunLiState::LK_Jump || stateType_ == ChunLiState::LK_JumpBack || stateType_ == ChunLiState::LK_JumpFront ||
		stateType_ == ChunLiState::MK_Jump || stateType_ == ChunLiState::MK_JumpBack || stateType_ == ChunLiState::MK_JumpFront || stateType_ == ChunLiState::HK_Jump || stateType_ == ChunLiState::HK_JumpBack || stateType_ == ChunLiState::HK_JumpFront)
	{
		if (HITSTYLE::Burn == _hitStyle)
		{
			return ChangeState(ChunLiState::Burn);
		}
		return ChangeState(ChunLiState::HitWhenJump);
	}
	// 대미지 크기와 유형에 따른 모션 변경?
	switch (_hitStyle)
	{
	case CharacterHandle::HITSTYLE::HITUPPERLIGHT:
		return ChangeState(ChunLiState::HitUpperLight);
		break;
	case CharacterHandle::HITSTYLE::HITUPPERHEAVY:
		return ChangeState(ChunLiState::HitUpperHeavy);
		break;
	case CharacterHandle::HITSTYLE::DOWNLIGHT:
		return ChangeState(ChunLiState::HitDownLight);
		break;
	case CharacterHandle::HITSTYLE::DOWNHEAVY:
		return ChangeState(ChunLiState::HitDownHeavy);
		break;
	case CharacterHandle::HITSTYLE::CROUCH:
		return ChangeState(ChunLiState::HitWhenCrouch);
		break;
	case CharacterHandle::HITSTYLE::JUMP:
		return ChangeState(ChunLiState::HitWhenJump);
		break;
	case CharacterHandle::HITSTYLE::DOWN:
		return ChangeState(ChunLiState::Down);
		break;
	case CharacterHandle::HITSTYLE::BeThrownDown:
		return ChangeState(ChunLiState::BeThrownDown);
		break;
	case CharacterHandle::HITSTYLE::Burn:
		return ChangeState(ChunLiState::Burn);
	case CharacterHandle::HITSTYLE::MAX:
		break;
	default:
		break;
	}
}



void ChunLi::HitUpperHeavy()
{

	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;

			if (GetModifiedSpriteNumber(158) < mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(158) < mainSpriteRender_->GetCurAnimationFrame())
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
			return ChangeState(ChunLiState::Idle);
		}
		break;

	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(158) < mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(158) < mainSpriteRender_->GetCurAnimationFrame())
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
			return ChangeState(ChunLiState::Idle);
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void ChunLi::HitUpperLight()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			if (GetModifiedSpriteNumber(158) <= mainSpriteRender_->GetCurAnimationFrame())
			{
				Sleep(5);
				SetMove(float4::LEFT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (85.0f >= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(158) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(ChunLiState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(158) <= mainSpriteRender_->GetCurAnimationFrame())
			{
				Sleep(5);
				SetMove(float4::RIGHT * knockBackDir_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
				if (knockBackDir_ >= 0.0f)
				{
					knockBackDir_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f;
				}
			}
		}
		if (1067.0f <= GetCameraLinkedPos().x)
		{
			if (GetModifiedSpriteNumber(158) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(ChunLiState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void ChunLi::HitDownHeavy()
{
	switch (dirstate_) // 넉백용 스위치문
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		if (85.0f < GetCameraLinkedPos().x)
		{
			curDirectionX_ = -1;
			if (GetModifiedSpriteNumber(157) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(157) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(ChunLiState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		if (1067.0f > GetCameraLinkedPos().x)
		{
			curDirectionX_ = 1;
			if (GetModifiedSpriteNumber(157) <= mainSpriteRender_->GetCurAnimationFrame())
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
			if (GetModifiedSpriteNumber(157) < mainSpriteRender_->GetCurAnimationFrame())
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
			ChangeState(ChunLiState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void ChunLi::HitDownLight()
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
			ChangeState(ChunLiState::Idle);
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
			ChangeState(ChunLiState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void ChunLi::HitWhenCrouch()
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
			ChangeState(ChunLiState::Idle);
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
			ChangeState(ChunLiState::Idle);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void ChunLi::HitWhenJump()
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
			return ChangeState(ChunLiState::HitWhenJumpDown);
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
			return ChangeState(ChunLiState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void ChunLi::HitWhenJumpDown()
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
			return ChangeState(ChunLiState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f });
			isJump_ = false;
			return ChangeState(ChunLiState::GetUp);
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
			return ChangeState(ChunLiState::Recovery);
		}

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd()) // 점프 후 바닥에 닿을 시
		{
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			SetDustParticle();
			SetPos({ GetPos().x, 450.0f }); // 오차가 나올 때를 대비해 강제로 바닥 y 값에 맞춰 줍니다.
			isJump_ = false;
			return ChangeState(ChunLiState::GetUp);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

}

void ChunLi::Down()
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
			return ChangeState(ChunLiState::GetUp);
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
			ChangeState(ChunLiState::GetUp);
			return;
		}
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}


void ChunLi::Burn()
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

		if (450.0f <= GetPos().iy()) // 점프 후 바닥에 닿을 시
		{
			SetDustParticle();
			soundPlayer_->PlayOverlap("Hittheground.wav", 0);
			return ChangeState(ChunLiState::HitWhenJumpDown);
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
			return ChangeState(ChunLiState::HitWhenJumpDown);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void ChunLi::GetUp()
{
	RemoveHitBox();
	RemoveBodyBox();
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::StandDelay);
	}
}


void ChunLi::Recovery()
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
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::KO()
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

void ChunLi::JumpKO()
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
			return ChangeState(ChunLiState::KO);
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
			return ChangeState(ChunLiState::KO);
		}
		return;
		break;

	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}


}

void ChunLi::Win()
{
	if ("P1" == GetName() && 1 <= FightScene::p1WinPoint_)
	{
		ChangeState(ChunLiState::MatchPointWin);
	}
	if ("P2" == GetName() && 1 <= FightScene::p2WinPoint_)
	{
		ChangeState(ChunLiState::MatchPointWin);
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}

void ChunLi::MatchPointWin()
{
	soundPlayer_->PlayOverlap("ChunLi_Win.mp3", 0);
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{

	}
}



void ChunLi::StandDelay() // 4프레임의 기상 디스어드밴티지 -> 이 행동 동안은 가드밖에 할 수 없다.
{
	RemoveBodyBox();
	RemoveHitBox();
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(ChunLiState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left") &&
		ACTORDIR::LOOK_RIGHT == dirstate_)
	{
		// 파동권이 존재한다는 것이니까.
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(ChunLiState::Guard);
		}
	}

	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrike_))
	{
		//if (InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MP") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HP")
		//	|| InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_LK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_MK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_HK") || InputManager::GetInputManager().IsButtonDown(otherPlayerTag_ + "_SP"))
		{
			return ChangeState(ChunLiState::Guard);
		}
	}
	else if (nullptr != otherCharacter_->projectile_ && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right") &&
		ACTORDIR::LOOK_LEFT == dirstate_)
	{
		if (nullptr != otherCharacter_->projectile_
			&& nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
		{
			return ChangeState(ChunLiState::Guard);
		}
	}

	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}

void ChunLi::LandDelay() // 점프 후 착지 시 딜레이
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ChunLiState::Idle);
	}
}


void ChunLi::Guard()
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
		return ChangeState(ChunLiState::CrouchGuard);
	}
	if (true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Up"))
	{
		ResetGuardBox();
		return ChangeState(ChunLiState::JumpBack);
	}
	if (chargingTimeBack_ >= 1.1f && true == InputManager::GetInputManager().IsButtonDown(playerTag_ + "_SP"))
	{
		ResetGuardBox();
		return ChangeState(ChunLiState::Kikoken);
	}

	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Right"))
	{
		return ChangeState(ChunLiState::Idle);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonUp(playerTag_ + "_Left"))
	{
		return ChangeState(ChunLiState::Idle);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR")
		/* && false == otherCharacter_->GetProjectile()->IsOn() */)
	{
		ResetGuardBox();
		return ChangeState(ChunLiState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL")
		/* && false == otherCharacter_->GetProjectile()->IsOn() */)
	{
		ResetGuardBox();
		return ChangeState(ChunLiState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Left"))
	{
		ResetGuardBox();
		return ChangeState(ChunLiState::Walk);
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && true == InputManager::GetInputManager().IsButtonPress(playerTag_ + "_Right"))
	{
		ResetGuardBox();
		return ChangeState(ChunLiState::Walk);
	}
}

void ChunLi::CrouchGuard()
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

		return ChangeState(ChunLiState::Guard);
	}
	if (nullptr != otherCharacter_->projectile_ && nullptr != guardRangeBox_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		return;
	}
	if (dirstate_ == ACTORDIR::LOOK_RIGHT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleR"))
	{
		return ChangeState(ChunLiState::Crouch);
	}
	if (dirstate_ == ACTORDIR::LOOK_LEFT && false == otherCharacter_->GetIsAttacking() && true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("IdleL"))
	{
		return ChangeState(ChunLiState::Crouch);
	}

}

void ChunLi::UpdateAfter()
{
	if (0 >= otherCharacter_->GetCurHp())
	{

		if (true == otherCharacter_->GetMainSpriteRenderer()->IsCurSpriteAnimationEnd())
		{
			if (true == mainSpriteRender_->IsCurSpriteAnimationName("Idle" + GetDirString()))
			{
				return ChangeState(ChunLiState::R1Win);
			}
			else if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
			{
				return ChangeState(ChunLiState::R1Win);
			}

		}
	}
}


void ChunLi::Collision()
{


}