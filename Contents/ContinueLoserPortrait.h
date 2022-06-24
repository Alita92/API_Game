#pragma once
#include <Object.h>

// 분류 :
// 용도 :
// 설명 :
class ContinueLoserPortrait : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	ContinueLoserPortrait(); // default constructer 디폴트 생성자
	~ContinueLoserPortrait(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ContinueLoserPortrait(const ContinueLoserPortrait& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ContinueLoserPortrait(ContinueLoserPortrait&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ContinueLoserPortrait& operator=(const ContinueLoserPortrait& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ContinueLoserPortrait& operator=(const ContinueLoserPortrait&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

public:
	void AnimationInit();
	void ChangePortrait(std::string _name);
};

