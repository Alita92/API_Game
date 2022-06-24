#pragma once
#include <Object.h>


enum class CURRENTCURSOR
{
	RYU,
	EHONDA,
	BLANKA,
	GUILE,
	BOXER,
	KEN,
	CHUNLI,
	ZANGIEF,
	DHALSIM,
	SAGAT,
	CLAW,
	THAWK,
	FEILONG,
	DEEJAY,
	CAMMY,
	DICTATOR,
	MAX
};

class EngineGeneralSoundPlayer;
class CharacterSelectScene;
class SelectionPortrait;
class SelectionFlag;
class Cursor : public Object
{
	friend SelectionPortrait;
	friend CharacterSelectScene;
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
	EngineGeneralSoundPlayer* soundPlayer_;
	SelectionPortrait* parentPortrait_;
	SelectionFlag* parentFlag_;
	std::string playerTag_;
	float cursorMoveRatio_;

	CURRENTCURSOR currentCursor_;

	bool isReady_;

	Cursor* otherCursor_;

private:
	std::string cursorLocation_;
	std::string selectedCharacter_;

public:
	void SetParentPortrait(SelectionPortrait* _portrait)
	{
		parentPortrait_ = _portrait;
	}

	void SetParentFlag(SelectionFlag* _flag)
	{
		parentFlag_ = _flag;
	}

	std::string ReturnLocation()
	{
		return cursorLocation_;
	}


public:
	void AnimationInit();
	void PlayerInit();

public:		
	Cursor(); // default constructer 디폴트 생성자
	~Cursor(); // default destructer 디폴트 소멸자

public:		// delete constructer
	Cursor(const Cursor& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Cursor(Cursor&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	Cursor& operator=(const Cursor& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Cursor& operator=(const Cursor&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Render() override;

public:
	void SetOtherCursor(Cursor* _other)
	{
		otherCursor_ = _other;
	}

	std::string CharacterSelected()
	{
		return selectedCharacter_;
	}
};

