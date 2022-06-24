#pragma once
#include <Object.h>
// 분류 : VS Scene 의 배경 화면입니다.
// 용도 : 뒷 배경을 단색으로 깔아줍니다.
// 설명 : RGB 0,0,170

class VSBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	VSBackground(); // default constructer 디폴트 생성자
	~VSBackground(); // default destructer 디폴트 소멸자

public:		// delete constructer
	VSBackground(const VSBackground& _other) = delete; // default Copy constructer 디폴트 복사생성자
	VSBackground(VSBackground&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	VSBackground& operator=(const VSBackground& _other) = delete; // default Copy operator 디폴트 대입 연산자
	VSBackground& operator=(const VSBackground&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

