#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class End : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	End(); // default constructer 디폴트 생성자
	~End(); // default destructer 디폴트 소멸자

public:		// delete constructer
	End(const End& _other) = delete; // default Copy constructer 디폴트 복사생성자
	End(End&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	End& operator=(const End& _other) = delete; // default Copy operator 디폴트 대입 연산자
	End& operator=(const End&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
};

