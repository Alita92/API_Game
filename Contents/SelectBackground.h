#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class SelectBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	SelectBackground(); // default constructer 디폴트 생성자
	~SelectBackground(); // default destructer 디폴트 소멸자

public:		// delete constructer
	SelectBackground(const SelectBackground& _other) = delete; // default Copy constructer 디폴트 복사생성자
	SelectBackground(SelectBackground&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	SelectBackground& operator=(const SelectBackground& _other) = delete; // default Copy operator 디폴트 대입 연산자
	SelectBackground& operator=(const SelectBackground&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

