#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :

class Cursor;
class SelectionPortrait : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
	Cursor* parentCursor_;

public:
	void AnimationInit();
	void PlayerInit();

	void SetParentCursor(Cursor* _cursor)
	{
		parentCursor_ = _cursor;
	}

	void ChangePortrait(std::string _name);

public:		
	SelectionPortrait(); // default constructer 디폴트 생성자
	~SelectionPortrait(); // default destructer 디폴트 소멸자

public:		// delete constructer
	SelectionPortrait(const SelectionPortrait& _other) = delete; // default Copy constructer 디폴트 복사생성자
	SelectionPortrait(SelectionPortrait&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	SelectionPortrait& operator=(const SelectionPortrait& _other) = delete; // default Copy operator 디폴트 대입 연산자
	SelectionPortrait& operator=(const SelectionPortrait&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;
};

