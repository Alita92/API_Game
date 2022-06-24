#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class StageGuileFog : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	StageGuileFog(); // default constructer 디폴트 생성자
	~StageGuileFog(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageGuileFog(const StageGuileFog& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageGuileFog(StageGuileFog&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageGuileFog& operator=(const StageGuileFog& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageGuileFog& operator=(const StageGuileFog&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

