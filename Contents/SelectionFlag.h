#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class SelectionFlag : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	SelectionFlag(); // default constructer 디폴트 생성자
	~SelectionFlag(); // default destructer 디폴트 소멸자

public:		// delete constructer
	SelectionFlag(const SelectionFlag& _other) = delete; // default Copy constructer 디폴트 복사생성자
	SelectionFlag(SelectionFlag&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	SelectionFlag& operator=(const SelectionFlag& _other) = delete; // default Copy operator 디폴트 대입 연산자
	SelectionFlag& operator=(const SelectionFlag&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
	
public:
	void AnimationInit();
	void PlayerInit();

public:
	void ChangeFlagAndPos(std::string _flag, float4 _pos);
};

