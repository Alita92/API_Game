#pragma once
#include <Object.h>
#include "CharacterHandle.h"
// 분류 :
// 용도 :
// 설명 :
class Shadow : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
	CharacterHandle* parentCharacter_;

public:
	void SetParentCharacter(CharacterHandle* _parent)
	{
		parentCharacter_ = _parent;
	}

	void PosInit();

public:		
	Shadow(); // default constructer 디폴트 생성자
	~Shadow(); // default destructer 디폴트 소멸자

public:		// delete constructer
	Shadow(const Shadow& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Shadow(Shadow&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	Shadow& operator=(const Shadow& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Shadow& operator=(const Shadow&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
};

