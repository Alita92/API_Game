#pragma once
#include <Scene.h>
// 분류 : 구체화된 Scene 으로, 1P와 2P 중 패배를 리턴받은 플레이어의 플레이 지속 여부를 묻습니다.
// 용도 : 제한 시간(10초) 가 주어지며, 실시간으로 카운트 숫자 오브젝트가 업데이트되어 표시됩니다.
//		  1P와 2P 중 패배를 리턴받은 플레이어가 직전에 선택한 캐릭터의 다친 포트레이트가 표시됩니다.
//		  제한 시간 초과 전 <스타트> 입력이 있으면 포트레이트가 흰 색으로 점멸 후 노멀 포트레이트가 표시됩니다.
// 
// 설명 : 제한 시간 초과 전 <스타트> 입력이 있으면 "CharacterSelectScene" 으로 씬을 전환합니다.
//		  제한 시간 초과 전까지 <스타트> 입력이 없으면 "TitleScene" 으로 씬을 전환합니다.
class ScreenFade;
class ContinueKOCounter;
class ContinueLoserPortrait;
class EngineGeneralSoundPlayer;
class ContinueScene : public Scene
{
private:	// member Var
	EngineGeneralSoundPlayer* bgmPlayer_;
	EngineGeneralSoundPlayer* soundPlayer_;
	float interTime_;
	float interTimeToChangeScene_;
	int timeSwitchNum_;
	bool isBgmPlaying_;

	bool isGameOver_;
	bool isSceneChange_;
	ContinueKOCounter* continueKOCounter_;
	ContinueLoserPortrait* continueLoserPortrait_;
	static ScreenFade* fadeObject_;

public:		
	ContinueScene(); // default constructer 디폴트 생성자
	~ContinueScene(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ContinueScene(const ContinueScene& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ContinueScene(ContinueScene&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ContinueScene& operator=(const ContinueScene& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ContinueScene& operator=(const ContinueScene&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Loading() override;
	void SceneUpdateBefore() override;
	void SceneUpdate() override;
};

