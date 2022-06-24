#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class StageGuilePeople : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageGuilePeople(); // default constructer 디폴트 생성자
	~StageGuilePeople(); // default destructer 디폴트 소멸자

public:		// delete constructer
	StageGuilePeople(const StageGuilePeople& _other) = delete; // default Copy constructer 디폴트 복사생성자
	StageGuilePeople(StageGuilePeople&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	StageGuilePeople& operator=(const StageGuilePeople& _other) = delete; // default Copy operator 디폴트 대입 연산자
	StageGuilePeople& operator=(const StageGuilePeople&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void Render() override;
	void Update() override;

public :
	void AnimationInIt();

public:
	void ChangeAnimation(std::string _name);
};

