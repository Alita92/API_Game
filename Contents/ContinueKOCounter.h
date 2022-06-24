#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class ContinueKOCounter : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	ContinueKOCounter(); // default constructer 디폴트 생성자
	~ContinueKOCounter(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ContinueKOCounter(const ContinueKOCounter& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ContinueKOCounter(ContinueKOCounter&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ContinueKOCounter& operator=(const ContinueKOCounter& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ContinueKOCounter& operator=(const ContinueKOCounter&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

	void AnimationInit();
	void ChangeNumber(std::string _number);
};

