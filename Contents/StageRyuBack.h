#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class StageRyuBack : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;

public:		
	StageRyuBack(); // default constructer 디폴트 생성자
	~StageRyuBack(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageRyuBack(const StageRyuBack& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageRyuBack(StageRyuBack&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageRyuBack& operator=(const StageRyuBack& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageRyuBack& operator=(const StageRyuBack&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;

};

