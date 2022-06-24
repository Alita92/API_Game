#include "Projectile.h"
#include <SpriteRenderer.h>
#include <EngineGeneralMath.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralSound.h>
#include <EngineGeneralSoundPlayer.h>
#include "Ryu.h"
#include "FightEffects.h"
// Static Var
// Static Func

// constructer destructer
Projectile::Projectile()
	: projectileSpeed_(450.0f)
{
	SetRenderOrder(30);
}

Projectile::~Projectile()
{

}

Projectile::Projectile(Projectile&& _other) noexcept
	: projectileSpeed_(450.0f)
{

}

//member Func
void Projectile::SetShotDir() // 장풍 시전 전 "해당 캐릭터" 가 실행할 수 있게 해 주세요.
{
	shotDir_ = parentCharacter_->dirstate_;


	switch (shotDir_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		SetPos({ parentCharacter_->GetPos().x + 150.0f, parentCharacter_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		SetPos({ parentCharacter_->GetPos().x - 150.0f, parentCharacter_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void Projectile::ChangeState(ProjectileState _state)
{
	if (_state == ProjectileState::MAX)
	{
		return;
	}

	state_ = _state;

	switch (state_)
	{
	case ProjectileState::FIRE:
		//strikeBoxProjectile_->SetCollisionCount(0);
		stateName_ = "HadoukenFire";
		mainSpriteRender_->ChangeSpriteAnimation(stateName_ + GetDirString(), true);
		break;
	case ProjectileState::FLYING:
		//strikeBoxProjectile_->SetCollisionCount(1);
		stateName_ = "HadoukenFlying";
		mainSpriteRender_->ChangeSpriteAnimation(stateName_ + GetDirString(), true);
		break;
	case ProjectileState::HIT:
		//strikeBoxProjectile_->SetCollisionCount(0);
		stateName_ = "HadoukenHit";
		mainSpriteRender_->ChangeSpriteAnimation(stateName_ + GetDirString(), true);
		break;
	case ProjectileState::MAX:
		break;
	default:
		break;
	}
}

std::string Projectile::GetDirString() // 업데이트로 체크된 "지향 방향" 을 측정해 글자를 리턴
{
	switch (shotDir_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		return "L";
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		return "R";
	default:
		EngineGeneralDebug::GameDestroyer();
		break;
	}
	EngineGeneralDebug::GameDestroyer();
	return "";
}

void Projectile::CollisionInit()
{
	if ("P1Projectile" == GetName())
	{
		strikeBoxProjectile_ = CreateCollision(ST2GROUP::PLAYER1PROJECTILE, ColliderType::RECT);
		enemyGuard_ = ST2GROUP::PLAYER2GUARD;
		enemyHit_ = ST2GROUP::PLAYER2HIT;
		enemyCrouchGuard_ = ST2GROUP::PLAYER2CROUCHGUARD;
		enemyStrikeProjectile_ = ST2GROUP::PLAYER2PROJECTILE;

	}
	if ("P2Projectile" == GetName())
	{
		strikeBoxProjectile_ = CreateCollision(ST2GROUP::PLAYER2PROJECTILE, ColliderType::RECT);
		enemyGuard_ = ST2GROUP::PLAYER1GUARD;
		enemyHit_ = ST2GROUP::PLAYER1HIT;
		enemyCrouchGuard_ = ST2GROUP::PLAYER1CROUCHGUARD;
		enemyStrikeProjectile_ = ST2GROUP::PLAYER1PROJECTILE;
	}

	strikeBoxProjectilePivot_ = { 0,0 };
	strikeBoxProjectile_->SetSize({ 120,90 });
	strikeBoxProjectile_->SetCameraEffectOn();
	ChangeState(ProjectileState::FIRE);
}

void Projectile::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("SpritesRyu.bmp");
	
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFireL", "SpritesRyu.bmp", 136, 137, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFireR", "SpritesRyu.bmp", 136 + 210, 137 + 210, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFlyingL", "SpritesRyu.bmp", 139, 141, true, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFlyingR", "SpritesRyu.bmp", 139 + 210, 141 + 210, true, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenHitL", "SpritesRyu.bmp", 143, 146, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenHitR", "SpritesRyu.bmp", 143 + 210, 146 + 210, false, 0.102f);
}


void Projectile::ConvertToKikoken()
{
	mainSpriteRender_ = CreateSpriteRenderer("SpritesChunLi.bmp");

	mainSpriteRender_->CreateSpriteAnimation("HadoukenFireL", "SpritesChunLi.bmp", 112, 113, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFireR", "SpritesChunLi.bmp", 112 + 210, 113 + 210, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFlyingL", "SpritesChunLi.bmp", 112, 116, true, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFlyingR", "SpritesChunLi.bmp", 112 + 210, 116 + 210, true, 0.051f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenHitL", "SpritesChunLi.bmp", 132, 135, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenHitR", "SpritesChunLi.bmp", 132 + 210, 135 + 210, false, 0.102f);

	projectileSpeed_ = 350.0f;
}

void Projectile::LastCheck()
{
	AnimationInit();
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	soundPlayer_->PlayCountReset(1);
	ChangeState(ProjectileState::FIRE);
}
void Projectile::UpdateBefore()
{

}
void Projectile::Update()
{
	switch (state_)
	{
	case ProjectileState::FIRE:
		Fire();
		break;
	case ProjectileState::FLYING:
		Flying();
		break;
	case ProjectileState::HIT:
		Hit();
		break;
	case ProjectileState::MAX:
		break;
	default:
		break;
	}
}
void Projectile::UpdateAfter()
{

}
void Projectile::Render()
{
	//strikeBoxProjectile_->StartDebugRender();
	mainSpriteRender_->FinalAnimationUpdate();
}

void Projectile::Collision()
{

}

void Projectile::Fire()
{
	isHit_ = false;
	isOneShot_ = true;
	shotDir_ = parentCharacter_->dirstate_;

	switch (shotDir_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		SetPos({ parentCharacter_->GetPos().x + 150.0f, parentCharacter_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		SetPos({ parentCharacter_->GetPos().x - 150.0f, parentCharacter_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		return ChangeState(ProjectileState::FLYING);
	}
}

void Projectile::Flying()
{
	parentCharacter_->isAttacking_ = true;

	if (nullptr != strikeBoxProjectile_->CollisionGroupCheckSingle(enemyGuard_) || nullptr != strikeBoxProjectile_->CollisionGroupCheckSingle(enemyCrouchGuard_))
	{
		soundPlayer_->PlayOverlap("Block.wav", 0);
		parentCharacter_->otherCharacter_->GetInstantDamage(1);
		parentCharacter_->isAttacking_ = false;
		return ChangeState(ProjectileState::HIT);
	}
	if (nullptr != strikeBoxProjectile_->CollisionGroupCheckSingle(enemyStrikeProjectile_))
	{
		parentCharacter_->isAttacking_ = false;
		return ChangeState(ProjectileState::HIT);
	}
	else if (nullptr != strikeBoxProjectile_->CollisionGroupCheckSingle(enemyHit_) && false == isHit_)
	{
		soundPlayer_->PlayOverlap("CriticalHit.wav", 0);
		parentCharacter_->otherCharacter_->SetDamaged(22, HITSTYLE::DOWNHEAVY); // 인자 (피해량, 피격 모션)
		isHit_ = true;
		parentCharacter_->isAttacking_ = false;

		return ChangeState(ProjectileState::HIT);

	}
	switch (shotDir_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
	{
		SetMove(float4::RIGHT * projectileSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
	}
	break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
	{
		SetMove(float4::LEFT * projectileSpeed_ * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
	}
	break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

	if (1152.0f <= GetPos().x || -100.0f >= GetPos().x)
	{
		parentCharacter_->projectile_ = nullptr;

		this->Death();

		isOneShot_ = false;
	}




}

void Projectile::Hit()
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		parentCharacter_->projectile_ = nullptr;

		this->Death();

		isOneShot_ = false;
	}
}