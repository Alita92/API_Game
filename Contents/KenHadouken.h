#pragma once
#include "CharacterHandle.h"
// 분류 :
// 용도 :
// 설명 :
enum class KenHadoukenState
{
	FIRE,
	FLYING,
	HIT,
	MAX
};

class KenHadouken : public CharacterHandle
{
private:	// member Var
private:	// member Var
	int damage_;

	CharacterHandle* parent_;
	ACTORDIR shotDir_;
	std::string stateName_;
	bool isHit_;
public:
	KenHadoukenState state_;

public:
	void SetParentCharacter(CharacterHandle* _parent)
	{
		parent_ = _parent;
	}

	void SetShotDir(); // 장풍 시전 전 "해당 캐릭터" 가 실행할 수 있게 해 주세요.
	std::string GetDirString();
public:		
	KenHadouken(); // default constructer 디폴트 생성자
	~KenHadouken(); // default destructer 디폴트 소멸자

public:		// delete constructer
	KenHadouken(const KenHadouken& _other) = delete; // default Copy constructer 디폴트 복사생성자
	KenHadouken(KenHadouken&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	KenHadouken& operator=(const KenHadouken& _other) = delete; // default Copy operator 디폴트 대입 연산자
	KenHadouken& operator=(const KenHadouken&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void Fire();
	void Flying();
	void Hit();

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void Collision() override;

	void AnimationInit() override;
	void CollisionInit();

	void ChangeState(KenHadoukenState _state);

	void SetStrikeBox(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
		float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation);
};

