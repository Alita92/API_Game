#pragma once
#include "CharacterHandle.h"

enum class CammyState
{
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
	HitWhenJump,
	HitWhenJumpDown,
	BeThrown,
	BeThrownDown,
	Down,
	Burn,
	Recovery,
	GetUp,
	KO,
	JumpKO,
	R1Win,
	MatchPointWin,
	StandDelay,
	LandDelay,

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

	Throw,
	SpiralArrow,
	CannonSpike,
	SpinBackKnuckle,
	NONE,
	MAX
};

class EngineGeneralSoundPlayer;
class Cammy : public CharacterHandle
{
private:	// member Var
	std::string StateName;
	CharacterHandle* linkedHandle_;

	bool isJumpUp_;
	bool isDebugOn_;


public:		
	Cammy(); // default constructer 디폴트 생성자
	~Cammy(); // default destructer 디폴트 소멸자

public:		// delete constructer
	Cammy(const Cammy& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Cammy(Cammy&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	Cammy& operator=(const Cammy& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Cammy& operator=(const Cammy&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

public:
	CammyState stateType_;

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

	void LP(); // 근접 기본기는 같은 기본기 함수 내에서 처리됩니다.
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

	void SpiralArrow();
	void CannonSpike();
	void SpinBackKnuckle();

	void Throw();

	void LeftRightCheck();
	std::string GetDirString();

public:
	void ChangeState(CammyState _stateType);

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
			return _number + 220;
		}
		return _number;
	}

};

