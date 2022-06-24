#include "KenHadouken.h"
#include <SpriteRenderer.h>
#include <EngineGeneralMath.h>
#include <EngineGeneralTime.h>
#include "Ken.h"
// Static Var
// Static Func

// constructer destructer
KenHadouken::KenHadouken()
{
	SetPos({ 0, 1500 });
	SetRenderOrder(30);
}

KenHadouken::~KenHadouken()
{
}

KenHadouken::KenHadouken(KenHadouken&& _other) noexcept
{
}

void KenHadouken::SetShotDir() // 장풍 시전 전 "해당 캐릭터" 가 실행할 수 있게 해 주세요.
{
	shotDir_ = parent_->dirstate_;

	switch (shotDir_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		SetPos({ parent_->GetPos().x + 150.0f, parent_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		SetPos({ parent_->GetPos().x - 150.0f, parent_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
}

void KenHadouken::ChangeState(KenHadoukenState _state)
{
	if (_state == KenHadoukenState::MAX)
	{
		return;
	}

	state_ = _state;

	switch (state_)
	{
	case KenHadoukenState::FIRE:
		strikeBoxProjectile_->SetCollisionCount(0);
		stateName_ = "HadoukenFire";
		mainSpriteRender_->ChangeSpriteAnimation(stateName_ + GetDirString(), true);
		break;
	case KenHadoukenState::FLYING:
		strikeBoxProjectile_->SetCollisionCount(1);
		stateName_ = "HadoukenFlying";
		mainSpriteRender_->ChangeSpriteAnimation(stateName_ + GetDirString(), true);
		break;
	case KenHadoukenState::HIT:
		strikeBoxProjectile_->SetCollisionCount(0);
		stateName_ = "HadoukenHit";
		mainSpriteRender_->ChangeSpriteAnimation(stateName_ + GetDirString(), true);
		break;
	case KenHadoukenState::MAX:
		break;
	default:
		break;
	}
}

std::string KenHadouken::GetDirString() // 업데이트로 체크된 "지향 방향" 을 측정해 글자를 리턴
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

void KenHadouken::CollisionInit()
{
	if ("P1Projectile" == GetName())
	{
		strikeBoxProjectile_ = CreateCollision(ST2GROUP::PLAYER1STRIKE, ColliderType::RECT);
		enemyGuard_ = ST2GROUP::PLAYER2GUARD;
		enemyHit_ = ST2GROUP::PLAYER2HIT;
		enemyCrouchGuard_ = ST2GROUP::PLAYER2CROUCHGUARD;

	}
	if ("P2Projectile" == GetName())
	{
		strikeBoxProjectile_ = CreateCollision(ST2GROUP::PLAYER2STRIKE, ColliderType::RECT);
		enemyGuard_ = ST2GROUP::PLAYER1GUARD;
		enemyHit_ = ST2GROUP::PLAYER1HIT;
		enemyCrouchGuard_ = ST2GROUP::PLAYER1CROUCHGUARD;
	}

	strikeBoxProjectilePivot_ = { 0,0 };
	strikeBoxProjectile_->SetSize({ 120,90 });
	strikeBoxProjectile_->SetCameraEffectOn();
	ChangeState(KenHadoukenState::FIRE);
}

void KenHadouken::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("SpritesRyu.bmp");

	mainSpriteRender_->CreateSpriteAnimation("HadoukenFireL", "SpritesRyu.bmp", 136, 137, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFireR", "SpritesRyu.bmp", 136 + 210, 137 + 210, false, 0.034f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFlyingL", "SpritesRyu.bmp", 139, 141, true, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenFlyingR", "SpritesRyu.bmp", 139 + 210, 141 + 210, true, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenHitL", "SpritesRyu.bmp", 143, 146, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("HadoukenHitR", "SpritesRyu.bmp", 143 + 210, 146 + 210, false, 0.102f);
}

void KenHadouken::LastCheck()
{
	AnimationInit();
	Off();
}
void KenHadouken::UpdateBefore()
{

}
void KenHadouken::Update()
{
	if (true == IsOn())
	{
		switch (state_)
		{
		case KenHadoukenState::FIRE:
			Fire();
			break;
		case KenHadoukenState::FLYING:
			Flying();
			break;
		case KenHadoukenState::HIT:
			Hit();
			break;
		case KenHadoukenState::MAX:
			break;
		default:
			break;
		}
	}
}
void KenHadouken::UpdateAfter()
{

}
void KenHadouken::Render()
{
	if (true == IsOn())
	{
		//strikeBoxProjectile_->StartDebugRender();
		mainSpriteRender_->FinalAnimationUpdate();
	}
}

void KenHadouken::Collision()
{

}

void KenHadouken::Fire()
{
	isHit_ = false;
	shotDir_ = parent_->dirstate_;

	switch (shotDir_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
		SetPos({ parent_->GetPos().x + 150.0f, parent_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
		SetPos({ parent_->GetPos().x - 150.0f, parent_->GetPos().y - 15.0f });
		break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		strikeBoxProjectile_->SetCollisionCount(0);
		return ChangeState(KenHadoukenState::FLYING);
	}
}

void KenHadouken::Flying()
{
	parent_->isAttacking_ = true;
	if (nullptr != strikeBoxProjectile_->CollisionGroupCheckSingle(enemyGuard_) || nullptr != strikeBoxProjectile_->CollisionGroupCheckSingle(enemyCrouchGuard_))
	{
		parent_->otherCharacter_->GetInstantDamage(1);
		parent_->isAttacking_ = false;
		return ChangeState(KenHadoukenState::HIT);
	}
	else if (nullptr != strikeBoxProjectile_->CollisionGroupCheckSingle(enemyHit_) && false == isHit_)
	{

		parent_->otherCharacter_->SetDamaged(22, HITSTYLE::DOWNHEAVY); // 인자 (피해량, 피격 모션)
		isHit_ = true;
		parent_->isAttacking_ = false;
		return ChangeState(KenHadoukenState::HIT);

	}
	switch (shotDir_)
	{
	case CharacterHandle::ACTORDIR::LOOK_RIGHT:
	{
		SetMove(float4::RIGHT * 450.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
	}
	break;
	case CharacterHandle::ACTORDIR::LOOK_LEFT:
	{
		SetMove(float4::LEFT * 450.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
	}
	break;
	case CharacterHandle::ACTORDIR::MAX:
		break;
	default:
		break;
	}

	if (1152.0f <= GetPos().x)
	{
		Off();
		parent_->isAttacking_ = false;
		return ChangeState(KenHadoukenState::FIRE);
	}


}

void KenHadouken::Hit()
{
	if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
	{
		Off();
		return ChangeState(KenHadoukenState::FIRE);
	}
}