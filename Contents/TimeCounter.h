#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class TimeCounter : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;
	float unitTime_;
	int roundTime_;

	enum class DIGITSTYLE
	{
		TENDIGIT,
		ONEDIGIT,
		MAX
	};

	std::string digitsTag_;
	DIGITSTYLE friendDigits_;

public:		
	TimeCounter(); // default constructer 디폴트 생성자
	~TimeCounter(); // default destructer 디폴트 소멸자

public:		// delete constructer
	TimeCounter(const TimeCounter& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TimeCounter(TimeCounter&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	TimeCounter& operator=(const TimeCounter& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TimeCounter& operator=(const TimeCounter&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

public:
	void spriteInit();
	void timeInit();
};

