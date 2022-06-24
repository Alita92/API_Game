#pragma once
#include <Object.h>

// 분류 :
// 용도 :
// 설명 :
class StageCammy : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageCammy(); // default constructer 디폴트 생성자
	~StageCammy(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageCammy(const StageCammy& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageCammy(StageCammy&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageCammy& operator=(const StageCammy& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageCammy& operator=(const StageCammy&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

	void AnimationInit();

};

