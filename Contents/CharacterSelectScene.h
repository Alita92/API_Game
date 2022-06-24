#pragma once
#include <Scene.h>

// 분류 : 구체화된 Scene 으로, 캐릭터 선택 화면입니다.
// 용도 : 지정된 제한 시간(30초) 이 주어지며, 지도 오브젝트와 캐릭터 포트레잇 오브젝트, 커서 등이 표시됩니다. 캐릭터 선택 음악이 재생됩니다.
// 설명 : 지정된 커서를 이동시켜 1P 와 2P 의 사용할 캐릭터를 선택합니다. 
//		  지정된 제한 시간(30초) 동안 캐릭터를 선택하지 않는다면 시간 초과 후 현 커서가 위치한 지점의 캐릭터를 자동으로 선택합니다.
//		  선택이 완료되었다면 오브젝트들이 페이드 아웃 처리되며 "VSScene" 으로 씬이 전환됩니다.\

class EngineGeneralSoundPlayer;
class VSTypo;
class SelectVSBackground;
class ScreenFade;
class Cursor;
class VSPortrait;
class VSName;
class CharacterSelectScene : public Scene
{
private:	// member Var
	Cursor* p1_;
	Cursor* p2_;

	std::string p1SelectedCharacter_;
	std::string p2SelectedCharacter_;

	static ScreenFade* fadeObject_;

	float interTime_;

	SelectVSBackground* vsBackground1_;
	SelectVSBackground* vsBackground2_;
	SelectVSBackground* vsBackground3_;
	VSTypo* vsTypo_;
	bool isFadein_;
	bool isVSin_;
	bool isSceneChange_;

	VSPortrait* p1VSPortrait_;
	VSPortrait* p2VSPortrait_;
	VSName* p1VSName_;
	VSName* p2VSName_;

	EngineGeneralSoundPlayer* bgmPlayer_;
	EngineGeneralSoundPlayer* soundPlayer_;
public:
	static std::string p1Selection_;
	static std::string p2Selection_;

public:
	static bool isSelectSceneNeedtobeReset_;

public:		
	CharacterSelectScene(); // default constructer 디폴트 생성자
	~CharacterSelectScene(); // default destructer 디폴트 소멸자

public:		// delete constructer
	CharacterSelectScene(const CharacterSelectScene& _other) = delete; // default Copy constructer 디폴트 복사생성자
	CharacterSelectScene(CharacterSelectScene&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	CharacterSelectScene& operator=(const CharacterSelectScene& _other) = delete; // default Copy operator 디폴트 대입 연산자
	CharacterSelectScene& operator=(const CharacterSelectScene&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Loading() override;
	void SceneUpdateBefore() override;
	void SceneUpdate() override;

	void SetP1Cursor(Cursor* _child)
	{
		p1_ = _child;
	}

	void SetP2Cursor(Cursor* _child)
	{
		p2_ = _child;
	}

	std::string GetP1Character()
	{
		return p1SelectedCharacter_;
	}

	std::string GetP2Character()
	{
		return p2SelectedCharacter_;
	}
};

