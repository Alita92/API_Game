#pragma once
#include <Object.h>

// 분류 :
// 용도 :
// 설명 :

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
	FightEffects(); // default constructer 디폴트 생성자
	~FightEffects(); // default destructer 디폴트 소멸자

public:		// delete constructer
	FightEffects(const FightEffects& _other) = delete; // default Copy constructer 디폴트 복사생성자
	FightEffects(FightEffects&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	FightEffects& operator=(const FightEffects& _other) = delete; // default Copy operator 디폴트 대입 연산자
	FightEffects& operator=(const FightEffects&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void Collision() override;
};

