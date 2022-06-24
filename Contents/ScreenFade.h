#pragma once
#include <Object.h>

// 분류 :
// 용도 :
// 설명 :
class ScreenFade : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;
	float alpha_;
	

	enum class FadeEnum
	{
		FADEIN, // 화면이 켜지는 것
		FADEOUT, // 화면이 꺼매지는 것
		MAX
	};

	FadeEnum fadeEnum_;

public:
	void FadeIn();
	void FadeOut();
	void ChangeState(FadeEnum _state);

public:
	void LastCheck() override;
	void Update() override;
	void Render() override;


public:		
	ScreenFade(); // default constructer 디폴트 생성자
	~ScreenFade(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ScreenFade(const ScreenFade& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ScreenFade(ScreenFade&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ScreenFade& operator=(const ScreenFade& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ScreenFade& operator=(const ScreenFade&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

};

