#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 : 
class FightBackGround : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	FightBackGround(); // default constructer 디폴트 생성자
	~FightBackGround(); // default destructer 디폴트 소멸자

public:		// delete constructer
	FightBackGround(const FightBackGround& _other) = delete; // default Copy constructer 디폴트 복사생성자
	FightBackGround(FightBackGround&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	FightBackGround& operator=(const FightBackGround& _other) = delete; // default Copy operator 디폴트 대입 연산자
	FightBackGround& operator=(const FightBackGround&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

