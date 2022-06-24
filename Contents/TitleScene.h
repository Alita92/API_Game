#pragma once
#include <Scene.h>

// 분류 : 구체화된 Scene 으로, 타이틀 화면입니다.
// 용도 : 인트로 동영상이 재생되며, 동영상 재생이 끝나면 타이틀 로고가 표시되며 하단에 "Insert Coin" 오브젝트가 점멸합니다.
//		  타이틀 로고 표시 후 아무 입력이 없으면 15초 후 인트로 동영상이 다시 재생됩니다.
// 설명 : 인트로 동영상 재생이 끝나기 전 <동전 투입> 버튼을 입력 시, 동영상이 스킵되고 바로 타이틀 로고 화면이 표시됩니다.
//		  <동전 투입> 버튼이 1회 이상 입력된 후에는, <스타트> 버튼을 입력하여 오브젝트들을 페이드 아웃 처리하고 "CharacterSelectScene" 으로 씬이 전환됩니다.

class TitleTypoSprites;
class ScreenFade;
class EngineGeneralSoundPlayer;
class TitleScene : public Scene
{
private:	// member Var
	static ScreenFade* fadeObject_;
	EngineGeneralSoundPlayer* soundPlayer_;

	float interTime_;
	bool isCoin_;
	bool isStart_;

	TitleTypoSprites* typo_;
public:		
	TitleScene(); // default constructer 디폴트 생성자
	~TitleScene(); // default destructer 디폴트 소멸자

public:		// delete constructer
	TitleScene(const TitleScene& _other) = delete; // default Copy constructer 디폴트 복사생성자
	TitleScene(TitleScene&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	TitleScene& operator=(const TitleScene& _other) = delete; // default Copy operator 디폴트 대입 연산자
	TitleScene& operator=(const TitleScene&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Loading() override;
	void SceneUpdate() override;
};

