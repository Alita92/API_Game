#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class DustParticle : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	DustParticle(); // default constructer 디폴트 생성자
	~DustParticle(); // default destructer 디폴트 소멸자

public:		// delete constructer
	DustParticle(const DustParticle& _other) = delete; // default Copy constructer 디폴트 복사생성자
	DustParticle(DustParticle&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	DustParticle& operator=(const DustParticle& _other) = delete; // default Copy operator 디폴트 대입 연산자
	DustParticle& operator=(const DustParticle&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
};

