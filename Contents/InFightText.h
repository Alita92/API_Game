#pragma once
#include <Object.h>

class InFightText : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

	float interTime_;
	bool isTextShown_;

public:
	InFightText(); // default constructer 디폴트 생성자
	~InFightText(); // default destructer 디폴트 소멸자

public:		// delete constructer
	InFightText(const InFightText& _other) = delete; // default Copy constructer 디폴트 복사생성자
	InFightText(InFightText&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	InFightText& operator=(const InFightText& _other) = delete; // default Copy operator 디폴트 대입 연산자
	InFightText& operator=(const InFightText&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetText(std::string _name);

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
};