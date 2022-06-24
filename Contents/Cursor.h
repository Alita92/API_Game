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
	Cursor(); // default constructer ����Ʈ ������
	~Cursor(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	Cursor(const Cursor& _other) = delete; // default Copy constructer ����Ʈ ���������
	Cursor(Cursor&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	Cursor& operator=(const Cursor& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Cursor& operator=(const Cursor&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

