#pragma once
#include "CharacterHandle.h"
// �з� :
// �뵵 :
// ���� :
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

	void SetShotDir(); // ��ǳ ���� �� "�ش� ĳ����" �� ������ �� �ְ� �� �ּ���.
	std::string GetDirString();
public:		
	KenHadouken(); // default constructer ����Ʈ ������
	~KenHadouken(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	KenHadouken(const KenHadouken& _other) = delete; // default Copy constructer ����Ʈ ���������
	KenHadouken(KenHadouken&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	KenHadouken& operator=(const KenHadouken& _other) = delete; // default Copy operator ����Ʈ ���� ������
	KenHadouken& operator=(const KenHadouken&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

