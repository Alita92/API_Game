#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class StageGuileFore : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageGuileFore(); // default constructer 디폴트 생성자
	~StageGuileFore(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageGuileFore(const StageGuileFore& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageGuileFore(StageGuileFore&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageGuileFore& operator=(const StageGuileFore& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageGuileFore& operator=(const StageGuileFore&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

