#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class StageGuileMid : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageGuileMid(); // default constructer 디폴트 생성자
	~StageGuileMid(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageGuileMid(const StageGuileMid& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageGuileMid(StageGuileMid&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageGuileMid& operator=(const StageGuileMid& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageGuileMid& operator=(const StageGuileMid&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void Update() override;
	void LastCheck() override;
};

