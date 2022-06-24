#pragma once
#include "CharacterHandle.h"
// �з� :
// �뵵 :
// ���� :

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

	void SetShotDir(); // ��ǳ ���� �� "�ش� ĳ����" �� ������ �� �ְ� �� �ּ���.
	std::string GetDirString();

public:
	Projectile(); // default constructer ����Ʈ ������
	~Projectile(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	Projectile(const Projectile& _other) = delete; // default Copy constructer ����Ʈ ���������
	Projectile(Projectile&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	Projectile& operator=(const Projectile& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Projectile& operator=(const Projectile&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

