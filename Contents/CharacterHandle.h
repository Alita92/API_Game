#pragma once
#include <Object.h>
#include <InputManager.h>
#include <Windows.h>
#include <EngineGeneralMath.h>
#include <GameCollision.h>
// 분류 :
// 용도 : 캐릭터의 원형이 되는 클래스입니다. 
// 설명 : 캐릭터로서 가져야 하는 기본 스탯과 상태를 상속해줍니다.
class Projectile;
class FightEffects;
class HPbar;
class EngineGeneralSoundPlayer;
class DustParticle;
class CharacterHandle : public Object
{
	friend HPbar;
	friend FightEffects;
	friend Projectile;
protected:
	// *** This 플레이어 것
	std::string playerTag_;
	HPbar* playerHPbar_;
	FightEffects* playerFightEffects_;

	EngineGeneralSoundPlayer* voicePlayer_;
	EngineGeneralSoundPlayer* soundPlayer_;

	int hp_;
	int strikeDamage_;
	float moveSpeed_;
	float jumpForce_;
	float maxJumpHeight_;
	bool isJump_;
	bool isAttacking_;
	bool isShooting_;
	bool isDown_;

	bool isWin_;

	int originalHp_;


	// *** 상대 플레이어 것
	std::string otherPlayerTag_;
	CharacterHandle* otherCharacter_;
public:
	Projectile* projectile_;

	int curDirectionX_;
	int curDirectionY_;

	bool isSessionOver_;

public: // 타 객체와의 연동용 함수
	void SetOtherCharacter(CharacterHandle* _other)
	{
		otherCharacter_ = _other;
	}

	void SetHPbar(HPbar* _hpbar)
	{
		playerHPbar_ = _hpbar;
	}

	void RestoreHP()
	{
		hp_ = originalHp_;
	}


	void SetFightEffects(FightEffects* _effects)
	{
		playerFightEffects_ = _effects;
	}

	void GetInstantDamage(int _damage) // 가드 대미지용
	{
		hp_ -= _damage;
	}


protected:
	enum class ST2GROUP // 콜리젼 그룹
	{
		GROUND,
		PLAYER1HIT,
		PLAYER1COLLISION,
		PLAYER1BODY,
		PLAYER1STRIKE,
		PLAYER1STRIKEOVERHEAD,
		PLAYER1STRIKELOW,
		PLAYER1GUARDRANGE,
		PLAYER1GUARD,
		PLAYER1CROUCHGUARD,
		PLAYER1PROJECTILE,
		PLAYER2HIT,
		PLAYER2COLLISION,
		PLAYER2BODY,
		PLAYER2STRIKE,
		PLAYER2STRIKEOVERHEAD,
		PLAYER2STRIKELOW,
		PLAYER2GUARDRANGE,
		PLAYER2GUARD,
		PLAYER2CROUCHGUARD,
		PLAYER2PROJECTILE,
		MAX
	};


	enum class ACTORDIR // 현재 캐릭터가 지향하고 있어야 할 방향
	{
		LOOK_RIGHT,
		LOOK_LEFT,
		MAX
	};

	enum class HITSTYLE
	{
		HITUPPERLIGHT,
		HITUPPERHEAVY,
		DOWNLIGHT,
		DOWNHEAVY,
		CROUCH,
		JUMP,
		DOWN,
		Throw,
		BeThrownDown,
		Burn,
		MAX
	};

	enum class STRIKESTYLE
	{
		HIGH,
		OVERHEAD,
		LOW,
		MAX

	};

protected:
	virtual void AnimationInit() = 0;

public:
	ACTORDIR dirstate_;
	HITSTYLE hitStyle_;
	std::string curHitstyle_;

protected:
	SpriteRenderer* mainSpriteRender_;

public:
	SpriteRenderer* GetMainSpriteRenderer()
	{
		return mainSpriteRender_;
	}

	int GetCurHp()
	{
		return hp_;
	}

	int GetCurStrikeDamage()
	{
		return strikeDamage_;
	}


	float GetMoveSpeed()
	{
		return moveSpeed_;
	}
	
	bool GetIsAttacking()
	{
		return isAttacking_;
	}

	bool GetIsJump()
	{
		return isJump_;
	}

	int GetCurDirectionX()
	{
		return curDirectionX_;
	}

	int GetCurDirectionY()
	{
		return curDirectionY_;
	}

	float GetJumpForce()
	{
		return jumpForce_;
	}

	std::string GetHitEffectLocation(std::string _hitStyle)
	{
		return _hitStyle;
	}

public:
	void RemoveCollisionBox()
	{
		collisionBoxPivot_ = { 0, 1000 };
		collisionBox_->SetPivot({ 0,1000 });
		collisionBox_->SetSize({ 0,0 });
	}
	void ResetCollisionBox()
	{
		collisionBox_->SetPivot(SetModifiedHitbox({ 0,15 }));
		collisionBox_->SetSize({ 90,220 });
	}

	void RemoveHitBox()
	{
		//hitBoxHeadPivot_ = { 0, 1000 };
		hitBoxTorsoPivot_ = { 0, 2000 };
		hitBoxTorso_->SetPivot(SetModifiedHitbox({ 0,2000 }));
		//hitBoxLegPivot_ = { 0, 1000 };
		//hitBoxHead_->SetSize(float4::ZERO);
		//hitBoxLeg_->SetSize(float4::ZERO);
		hitBoxTorso_->SetSize(float4::ZERO);
	}

	void RemoveBodyBox()
	{
		bodyBox_->SetPivot(SetModifiedHitbox({ 0,1000 }));
		bodyBox_->SetSize({ 0,0 });
	}


	// ***** 판정 박스
public:
	GameCollision* collisionBox_; // 물리적 몸체 판정 박스
	float4 collisionBoxPivot_;

	GameCollision* bodyBox_; // 몸 판정(잡기와 근접 기본기 발동 조건 전용) 박스 ***콜리전 박스보다 크게 만들 것
	float4 bodyBoxPivot_;

	GameCollision* hitBoxHead_; // 히트박스; 머리
	float4 hitBoxHeadPivot_;
	GameCollision* hitBoxTorso_; // 히트박스; 상체
	float4 hitBoxTorsoPivot_;
	GameCollision* hitBoxLeg_; // 히트박스; 하체
	float4 hitBoxLegPivot_;

	GameCollision* strikeBox_; // 공격 판정 박스(상단)
	float4 strikeBoxPivot_;

	GameCollision* strikeBoxOverHead_;  // 공격 판정 박스 (중단)
	float4 strikeBoxOverHeadPivot_;

	GameCollision* strikeBoxLow_; // 공격 판정 팍스 (하단)
	float4 strikeBoxLowPivot_;

	GameCollision* strikeBoxProjectile_; // 투사체 판정 박스 (기본은 상단입니다.)
	float4 strikeBoxProjectilePivot_;

	GameCollision* guardRangeBox_; // 가드 발동 범위 박스
	float4 guardRangeBoxPivot_;

	GameCollision* guardBox_;
	float4 guardBoxPivot_;

	GameCollision* crouchGuardBox_;
	float4 crouchGuardBoxPivot_;

	// 점프로
	float4 JumpDir;

	// 넉백가속도
	float knockBackDir_;


	// ***** 상대 판정박스 인식해 줄 변수
	ST2GROUP enemyBody_;
	ST2GROUP enemyCollision_;
	ST2GROUP enemyHit_;

	ST2GROUP enemyStrike_;
	ST2GROUP enemyStrikeOverHead_;
	ST2GROUP enemyStrikeLow_;
	ST2GROUP enemyStrikeProjectile_;

	ST2GROUP enemyGuardRange_;
	ST2GROUP enemyGuard_;
	ST2GROUP enemyCrouchGuard_;

public:
	CharacterHandle(); // default constructer 디폴트 생성자
	~CharacterHandle(); // default destructer 디폴트 소멸자

public:		// delete constructer
	CharacterHandle(const CharacterHandle& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharacterHandle(CharacterHandle&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	CharacterHandle& operator=(const CharacterHandle& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharacterHandle& operator=(const CharacterHandle&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:


	virtual void SetDamaged(int _damage, HITSTYLE _hitStyle)
	{

	}

	virtual void SetHold(int _damage)
	{

	}

	float4 SetModifiedHitbox(float4 _pivot)
	{
		switch (dirstate_)
		{
		case CharacterHandle::ACTORDIR::LOOK_RIGHT:
			return _pivot;
			break;
		case CharacterHandle::ACTORDIR::LOOK_LEFT:
			return { -_pivot.x, _pivot.y };
			break;
		case CharacterHandle::ACTORDIR::MAX:
			return _pivot;
			break;
		default:
			return _pivot;
			break;
		}
		return _pivot;
	}

	void SetDustParticle();
	void SetInFightText();
	void SetReversalText();
};