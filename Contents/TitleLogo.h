#pragma once
#include <Object.h>

// 분류 :
// 용도 :
// 설명 :
class TitleLogo : public Object
{
private:
	SpriteRenderer* mainSpriteRender_;


public:		
	TitleLogo(); // default constructer 디폴트 생성자
	~TitleLogo(); // default destructer 디폴트 소멸자

public:		// delete constructer
	TitleLogo(const TitleLogo& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitleLogo(TitleLogo&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	TitleLogo& operator=(const TitleLogo& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitleLogo& operator=(const TitleLogo&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func

	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	void LastCheck() override;
};