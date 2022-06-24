#pragma once
#include <Object.h>

// �з� :
// �뵵 :
// ���� :

class Projectile;
class CharacterHandle;
class FightEffects : public Object
{
	friend Projectile;
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
	CharacterHandle* parentCharacter_;

public:
	SpriteRenderer* GetMainSpriteRender()
	{
		return mainSpriteRender_;
	}

	void SetParentCharacter(CharacterHandle* _character)
	{
		parentCharacter_ = _character;
	}

public:		
	FightEffects(); // default constructer ����Ʈ ������
	~FightEffects(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	FightEffects(const FightEffects& _other) = delete; // default Copy constructer ����Ʈ ���������
	FightEffects(FightEffects&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	FightEffects& operator=(const FightEffects& _other) = delete; // default Copy operator ����Ʈ ���� ������
	FightEffects& operator=(const FightEffects&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void Collision() override;
};

