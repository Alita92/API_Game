#pragma once
#include "CharacterHandle.h"
#include <EngineGeneralMath.h>
enum class RyuState
{
	// L,R ������ �ϴ��� ENUM ACTORDIR class �� �����մϴ�.
	// ��� State
	Idle,
	Walk,
	Crouch,
	Jump,
	JumpFront,
	JumpBack,
	Guard,
	CrouchGuard,

	HitUpperLight,
	HitUpperHeavy,
	HitDownLight,
	HitDownHeavy,
	HitWhenCrouch,
	HitWhenJump, // ���ߴ��ؼ� �߶��Ҷ�
	HitWhenJumpDown, // �߶� �� �ٴڿ� ������ ��
	BeThrown, // ��⿡ ������ ��
	BeThrownDown,
	Down,
	Burn,
	Recovery,
	GetUp,
	KO,
	JumpKO,
	R1Win,
	MatchPointWin,
	StandDelay, // ��� ���Ŀ� ���̴� ������ ����Դϴ�.
	LandDelay, // ���� ���� ���Ŀ� ���̴� ������ ����Դϴ�.

	// �⺻�� State 
	// ���� �⺻��� ���� State �� �����մϴ�.
	LP,
	MP,
	HP,
	LK,
	MK,
	HK,
	LP_Crouch,
	LP_Close,
	LP_Jump,
	LP_JumpFront,
	LP_JumpBack,
	MP_Crouch,
	MP_Close,
	MP_Jump,
	MP_JumpFront,
	MP_JumpBack,
	HP_Crouch,
	HP_Close,
	HP_Jump,
	HP_JumpFront,
	HP_JumpBack,
	LK_Crouch,
	LK_Close,
	LK_Jump,
	LK_JumpFront,
	LK_JumpBack,
	MK_Crouch,
	MK_Close,
	MK_Jump,
	MK_JumpFront,
	MK_JumpBack,
	HK_Crouch,
	HK_Close,
	HK_Jump,
	HK_JumpFront,
	HK_JumpBack,

	// ��� �� �ʻ�� State
	Throw,
	Hadouken,
	Shoryuken,
	Tatsumaki,
	NONE, // �ƹ��͵� ���� �ʰ� �׳� ���ϸ�

	AIIdle,
	AIWalk,
	AICrouch,
	AIJump,
	AIJumpFront,
	AIJumpBack,

	MAX
};

class EngineGeneralSoundPlayer;
class Ryu : public CharacterHandle // ���ÿ� ������Ʈ�� ��ӹ޽��ϴ�.
{
private:
	friend Projectile;

	std::string StateName; // State ��ȯ �� ��ȯ�� State �� �ڵ�� �νĽ����� String �Դϴ�.
	CharacterHandle* linkedHandle_;

	bool isJumpUp_; // �ö󰡴� ��...
	bool isDebugOn_;
	
	// *** AI ***
	Random randomGenerator_;
	bool isAIOn_;
	int diceRoll_;

public:
	Ryu(); // default constructer ����Ʈ ������
	~Ryu() override; // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	Ryu(const Ryu& _other) = delete; // default Copy constructer ����Ʈ ���������
	Ryu(Ryu&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	Ryu& operator=(const Ryu& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Ryu& operator=(const Ryu&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void Collision() override;

public:
	void KeyInit();
	void AnimationInit() override;
	void ToggleDebug();
	void ToggleAI();

public:
	RyuState stateType_;

public:

	void Idle();
	void Walk();
	void Crouch();
	void Jump();
	void JumpFront();
	void JumpBack();
	void Guard();
	void CrouchGuard();

	void HitUpperHeavy();
	void HitUpperLight();
	void HitDownLight();
	void HitDownHeavy();
	void HitWhenCrouch();
	void HitWhenJump();
	void HitWhenJumpDown();
	void BeThrown();
	void BeThrownDown();
	void Down();
	void GetUp();
	void Burn();
	void Recovery();
	void KO();
	void JumpKO();
	void Win();
	void MatchPointWin();

	void StandDelay();
	void LandDelay();

	void LP(); // ���� �⺻��� ���� �⺻�� �Լ� ������ ó���˴ϴ�.
	void LP_Close();
	void LP_Crouch();
	void LP_Jump();
	void LP_JumpFront();
	void LP_JumpBack();

	void MP();
	void MP_Close();
	void MP_Crouch();
	void MP_Jump();
	void MP_JumpFront();
	void MP_JumpBack();

	void HP();
	void HP_Close();
	void HP_Crouch();
	void HP_Jump();
	void HP_JumpFront();
	void HP_JumpBack();

	void LK();
	void LK_Close();
	void LK_Crouch();
	void LK_Jump();
	void LK_JumpFront();
	void LK_JumpBack();

	void MK();
	void MK_Close();
	void MK_Crouch();
	void MK_Jump();
	void MK_JumpFront();
	void MK_JumpBack();

	void HK();
	void HK_Close();
	void HK_Crouch();
	void HK_Jump();
	void HK_JumpFront();
	void HK_JumpBack();

	void Hadouken();
	void Shoryuken();
	void Tatsumaki();

	void Throw();

	void LeftRightCheck();
	std::string GetDirString();

	// AI
public:
	void AIIdle();
	void AIWalk();
	void AICrouch();
	void AIJump();
	void AIJumpFront();
	void AIJumpBack();


public:
	void ChangeState(RyuState _stateType);

public:
	void SetStrikeBox(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder, 
		float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation);

	void SetStrikeBoxOverHead(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
		float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation);

	void SetStrikeBoxLow(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
		float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation);

	void ResetStrikeBox();
	void ResetGuardBox();
	void ResetHitBox();
	void ResetBodyBox();
	

	void SetDamaged(int _damage, HITSTYLE _hitStyle) override;
	void SetHold(int _damage) override;

	int GetModifiedSpriteNumber(int _number)
	{
		if (ACTORDIR::LOOK_LEFT == dirstate_)
		{
			return _number + 210;
		}
		return _number;
	}

};

