#include "CharacterHandle.h"
#include "DustParticle.h"
#include "InFightText.h"

// Static Var
// Static Func

// constructer destructer
CharacterHandle::CharacterHandle()
	: mainSpriteRender_(nullptr),
	bodyBoxPivot_(float4::ZERO),
	hitBoxHeadPivot_(float4::ZERO), hitBoxTorsoPivot_(float4::ZERO), hitBoxLegPivot_(float4::ZERO),
	strikeBoxPivot_(float4::ZERO), guardRangeBoxPivot_(float4::ZERO), strikeBoxOverHeadPivot_(float4::ZERO), strikeBoxLowPivot_(float4::ZERO),
	bodyBox_(nullptr), collisionBox_(nullptr), hitBoxHead_(nullptr), hitBoxTorso_(nullptr), hitBoxLeg_(nullptr), strikeBox_(nullptr), guardRangeBox_(nullptr), guardBox_(nullptr), crouchGuardBox_(nullptr),
	enemyBody_(ST2GROUP::MAX), enemyCollision_(ST2GROUP::MAX), enemyHit_(ST2GROUP::MAX), enemyStrike_(ST2GROUP::MAX), enemyGuardRange_(ST2GROUP::MAX), enemyGuard_(ST2GROUP::MAX),
	enemyCrouchGuard_(ST2GROUP::MAX), enemyStrikeLow_(ST2GROUP::MAX), enemyStrikeOverHead_(ST2GROUP::MAX), enemyStrikeProjectile_(ST2GROUP::MAX),
	hp_(0), isJump_(false), moveSpeed_(0), dirstate_(ACTORDIR::MAX),
	maxJumpHeight_(0.0f), otherCharacter_(nullptr), playerHPbar_(nullptr), strikeDamage_(0), jumpForce_(0.0f), curDirectionX_(0), curDirectionY_(0), isAttacking_(false), originalHp_(0),
	playerFightEffects_(nullptr), isWin_(false), knockBackDir_(300.0f), isShooting_(false), isDown_(false), isSessionOver_(false), hitStyle_(HITSTYLE::MAX), projectile_(nullptr), soundPlayer_(nullptr), strikeBoxLow_(nullptr),
	strikeBoxOverHead_(nullptr), strikeBoxProjectile_(nullptr), voicePlayer_(nullptr)
{
}

CharacterHandle::~CharacterHandle()
{
}

CharacterHandle::CharacterHandle(CharacterHandle&& _other) noexcept
: mainSpriteRender_(_other.mainSpriteRender_),
bodyBoxPivot_(_other.bodyBoxPivot_),
hitBoxHeadPivot_(_other.hitBoxHeadPivot_), hitBoxTorsoPivot_(_other.hitBoxTorsoPivot_), hitBoxLegPivot_(_other.hitBoxLegPivot_),
strikeBoxPivot_(_other.strikeBoxPivot_), guardRangeBoxPivot_(_other.guardRangeBoxPivot_),
bodyBox_(_other.bodyBox_), collisionBox_(_other.collisionBox_), hitBoxHead_(_other.hitBoxHead_), hitBoxTorso_(_other.hitBoxTorso_), hitBoxLeg_(_other.hitBoxLeg_), strikeBox_(_other.strikeBox_), guardRangeBox_(_other.guardRangeBox_), guardBox_(_other.guardBox_),crouchGuardBox_(nullptr),
enemyBody_(_other.enemyBody_), enemyCollision_(_other.enemyCollision_), enemyHit_(_other.enemyHit_), enemyStrike_(_other.enemyStrike_),enemyGuardRange_(_other.enemyGuardRange_), enemyGuard_(_other.enemyGuard_), hp_(_other.hp_), isJump_(false),  moveSpeed_(0), dirstate_(ACTORDIR::MAX),
enemyCrouchGuard_(ST2GROUP::MAX), enemyStrikeLow_(ST2GROUP::MAX), enemyStrikeOverHead_(ST2GROUP::MAX), enemyStrikeProjectile_(ST2GROUP::MAX),
 maxJumpHeight_(0.0f), otherCharacter_(nullptr), playerHPbar_(nullptr), strikeDamage_(0), jumpForce_(0.0f), curDirectionX_(0), curDirectionY_(0), isAttacking_(false), originalHp_(0),
	playerFightEffects_(_other.playerFightEffects_), isWin_(false), knockBackDir_(300.0f), isShooting_(false), isDown_(false), isSessionOver_(false), hitStyle_(HITSTYLE::MAX), projectile_(nullptr), soundPlayer_(nullptr), strikeBoxLow_(nullptr),
	strikeBoxOverHead_(nullptr), strikeBoxProjectile_(nullptr), voicePlayer_(nullptr)
{
}

//member Func

void CharacterHandle::SetDustParticle()
{
		DustParticle* particle1 = GetScene()->CreateObject<DustParticle>();
		particle1->SetPos({ GetPos().x - 21.0f, 570.0f });
		DustParticle* particle2 = GetScene()->CreateObject<DustParticle>();
		particle2->SetPos({ GetPos().x, 580.0f });
		DustParticle* particle3 = GetScene()->CreateObject<DustParticle>();
		particle3->SetPos({ GetPos().x + 37.0f , 560.0f });
}

void CharacterHandle::SetInFightText()
{
	if ("P1" == GetName())
	{
		InFightText* inFightText = GetScene()->CreateObject<InFightText>();
		inFightText->SetName("P1");
		inFightText->SetPos({-680.0f, 200.0f});
	}
	if ("P2" == GetName())
	{
		InFightText* inFightText = GetScene()->CreateObject<InFightText>();
		inFightText->SetName("P2");
		inFightText->SetPos({1832.0f, 200.0f});
	}
}

void CharacterHandle::SetReversalText()
{
	if ("P1" == GetName())
	{
		InFightText* inFightText = GetScene()->CreateObject<InFightText>();
		inFightText->SetName("P1");
		inFightText->SetPos({ -680.0f, 200.0f });
		inFightText->SetText("Reversal");
	}
	if ("P2" == GetName())
	{
		InFightText* inFightText = GetScene()->CreateObject<InFightText>();
		inFightText->SetName("P2");
		inFightText->SetPos({ 1832.0f, 200.0f });
		inFightText->SetText("Reversal");
	}
}