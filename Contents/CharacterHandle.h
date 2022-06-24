#pragma once
#include <Object.h>
#include <InputManager.h>
#include <Windows.h>
#include <EngineGeneralMath.h>
#include <GameCollision.h>
// �з� :
// �뵵 : ĳ������ ������ �Ǵ� Ŭ�����Դϴ�. 
// ���� : ĳ���ͷμ� ������ �ϴ� �⺻ ���Ȱ� ���¸� ������ݴϴ�.
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
	// *** This �÷��̾� ��
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


	// *** ��� �÷��̾� ��
	std::string otherPlayerTag_;
	CharacterHandle* otherCharacter_;
public:
	Projectile* projectile_;

	int curDirectionX_;
	int curDirectionY_;

	bool isSessionOver_;

public: // Ÿ ��ü���� ������ �Լ�
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

	void GetInstantDamage(int _damage) // ���� �������
	{
		hp_ -= _damage;
	}


protected:
	enum class ST2GROUP // �ݸ��� �׷�
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


	enum class ACTORDIR // ���� ĳ���Ͱ� �����ϰ� �־�� �� ����
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


	// ***** ���� �ڽ�
public:
	GameCollision* collisionBox_; // ������ ��ü ���� �ڽ�
	float4 collisionBoxPivot_;

	GameCollision* bodyBox_; // �� ����(���� ���� �⺻�� �ߵ� ���� ����) �ڽ� ***�ݸ��� �ڽ����� ũ�� ���� ��
	float4 bodyBoxPivot_;

	GameCollision* hitBoxHead_; // ��Ʈ�ڽ�; �Ӹ�
	float4 hitBoxHeadPivot_;
	GameCollision* hitBoxTorso_; // ��Ʈ�ڽ�; ��ü
	float4 hitBoxTorsoPivot_;
	GameCollision* hitBoxLeg_; // ��Ʈ�ڽ�; ��ü
	float4 hitBoxLegPivot_;

	GameCollision* strikeBox_; // ���� ���� �ڽ�(���)
	float4 strikeBoxPivot_;

	GameCollision* strikeBoxOverHead_;  // ���� ���� �ڽ� (�ߴ�)
	float4 strikeBoxOverHeadPivot_;

	GameCollision* strikeBoxLow_; // ���� ���� �Ž� (�ϴ�)
	float4 strikeBoxLowPivot_;

	GameCollision* strikeBoxProjectile_; // ����ü ���� �ڽ� (�⺻�� ����Դϴ�.)
	float4 strikeBoxProjectilePivot_;

	GameCollision* guardRangeBox_; // ���� �ߵ� ���� �ڽ�
	float4 guardRangeBoxPivot_;

	GameCollision* guardBox_;
	float4 guardBoxPivot_;

	GameCollision* crouchGuardBox_;
	float4 crouchGuardBoxPivot_;

	// ������
	float4 JumpDir;

	// �˹鰡�ӵ�
	float knockBackDir_;


	// ***** ��� �����ڽ� �ν��� �� ����
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
	CharacterHandle(); // default constructer ����Ʈ ������
	~CharacterHandle(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	CharacterHandle(const CharacterHandle& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharacterHandle(CharacterHandle&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	CharacterHandle& operator=(const CharacterHandle& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharacterHandle& operator=(const CharacterHandle&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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