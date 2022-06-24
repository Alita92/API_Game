#pragma once
#include "CharacterHandle.h"
// 분류 :
// 용도 :
// 설명 :

enum class ProjectileState
{
	FIRE,
	FLYING,
	HIT,
	MAX
};

class EngineGeneralSoundPlayer;
class Projectile : public CharacterHandle
{
private:	// member Var
	int damage_;

	EngineGeneralSoundPlayer* soundPlayer_;
	CharacterHandle* parentCharacter_;
	ACTORDIR shotDir_;
	std::string stateName_;
	bool isHit_;

	float projectileSpeed_;

public:
	bool isOneShot_;
public:
	ProjectileState state_;

public:
	void SetParentCharacter(CharacterHandle* _parent)
	{
		parentCharacter_ = _parent;
	}

	void SetShotDir(); // 장풍 시전 전 "해당 캐릭터" 가 실행할 수 있게 해 주세요.
	std::string GetDirString();

public:
	Projectile(); // default constructer 디폴트 생성자
	~Projectile(); // default destructer 디폴트 소멸자

public:		// delete constructer
	Projectile(const Projectile& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Projectile(Projectile&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	Projectile& operator=(const Projectile& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Projectile& operator=(const Projectile&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

	void ConvertToKikoken();

	void CollisionInit();

	void ChangeState(ProjectileState _state);

	void SetStrikeBox(SpriteRenderer* _renderer, int _targetFrame, int _exitFrame, int _spriteLRBorder,
		float4 _pivot, float4 _size, int _strikeDamage, HITSTYLE _hitStyle, std::string _hitLocation);
};

