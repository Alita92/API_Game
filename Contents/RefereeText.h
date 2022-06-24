#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class FightScene;
class RefereeText : public Object
{
	friend FightScene;
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;



public:		
	RefereeText(); // default constructer 디폴트 생성자
	~RefereeText(); // default destructer 디폴트 소멸자

public:		// delete constructer
	RefereeText(const RefereeText& _other) = delete; // default Copy constructer 디폴트 복사생성자
	RefereeText(RefereeText&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	RefereeText& operator=(const RefereeText& _other) = delete; // default Copy operator 디폴트 대입 연산자
	RefereeText& operator=(const RefereeText&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

public:
	void ChangeText(std::string _imageName);
	SpriteRenderer* GetMainSpriteRenderer()
	{
		return mainSpriteRenderer_;
	}
};

