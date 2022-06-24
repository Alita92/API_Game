#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class Winpoint : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	Winpoint(); // default constructer 디폴트 생성자
	~Winpoint(); // default destructer 디폴트 소멸자

public:		// delete constructer
	Winpoint(const Winpoint& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Winpoint(Winpoint&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	Winpoint& operator=(const Winpoint& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Winpoint& operator=(const Winpoint&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

};

