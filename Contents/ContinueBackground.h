#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class ContinueBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	ContinueBackground(); // default constructer 디폴트 생성자
	~ContinueBackground(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ContinueBackground(const ContinueBackground& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ContinueBackground(ContinueBackground&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ContinueBackground& operator=(const ContinueBackground& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ContinueBackground& operator=(const ContinueBackground&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Update() override;
	void Render() override;
	void LastCheck() override;
};

