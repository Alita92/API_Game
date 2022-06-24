#pragma once
#include <Object.h>
// 분류 :
// 용도 :
// 설명 :
class TitleTypoSprites : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	TitleTypoSprites(); // default constructer 디폴트 생성자
	~TitleTypoSprites(); // default destructer 디폴트 소멸자

public:		// delete constructer
	TitleTypoSprites(const TitleTypoSprites& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitleTypoSprites(TitleTypoSprites&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	TitleTypoSprites& operator=(const TitleTypoSprites& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitleTypoSprites& operator=(const TitleTypoSprites&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
	void ChangeSprite();
};

