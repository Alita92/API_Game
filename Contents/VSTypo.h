#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class VSTypo : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	VSTypo(); // default constructer 디폴트 생성자
	~VSTypo(); // default destructer 디폴트 소멸자

public:		// delete constructer
	VSTypo(const VSTypo& _other) = delete; // default Copy constructer 디폴트 복사생성자
	VSTypo(VSTypo&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	VSTypo& operator=(const VSTypo& _other) = delete; // default Copy operator 디폴트 대입 연산자
	VSTypo& operator=(const VSTypo&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
};

