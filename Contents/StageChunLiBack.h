#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class StageChunLiBack : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	StageChunLiBack(); // default constructer 디폴트 생성자
	~StageChunLiBack(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageChunLiBack(const StageChunLiBack& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageChunLiBack(StageChunLiBack&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageChunLiBack& operator=(const StageChunLiBack& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageChunLiBack& operator=(const StageChunLiBack&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

