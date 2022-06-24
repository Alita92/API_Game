#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class VSPortraitP2 : public Object
{
private:	// member Var

public:		
	VSPortraitP2(); // default constructer 디폴트 생성자
	~VSPortraitP2(); // default destructer 디폴트 소멸자

public:		// delete constructer
	VSPortraitP2(const VSPortraitP2& _other) = delete; // default Copy constructer 디폴트 복사생성자
	VSPortraitP2(VSPortraitP2&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	VSPortraitP2& operator=(const VSPortraitP2& _other) = delete; // default Copy operator 디폴트 대입 연산자
	VSPortraitP2& operator=(const VSPortraitP2&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
};

