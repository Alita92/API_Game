#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class SelectVSBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	SelectVSBackground(); // default constructer 디폴트 생성자
	~SelectVSBackground(); // default destructer 디폴트 소멸자

public:		// delete constructer
	SelectVSBackground(const SelectVSBackground& _other) = delete; // default Copy constructer 디폴트 복사생성자
	SelectVSBackground(SelectVSBackground&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	SelectVSBackground& operator=(const SelectVSBackground& _other) = delete; // default Copy operator 디폴트 대입 연산자
	SelectVSBackground& operator=(const SelectVSBackground&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

