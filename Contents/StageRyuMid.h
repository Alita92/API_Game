#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class StageRyuMid : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;


public:		
	StageRyuMid(); // default constructer 디폴트 생성자
	~StageRyuMid(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageRyuMid(const StageRyuMid& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageRyuMid(StageRyuMid&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageRyuMid& operator=(const StageRyuMid& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageRyuMid& operator=(const StageRyuMid&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void Update() override;
	void LastCheck() override;
};

