#pragma once
#include <Scene.h>
// 분류 : 구체화된 Scene 으로, 1P와 2P의 승패 리턴을 비교합니다.
// 용도 : 승패의 리턴을 비교해 승리한 플레이어의 캐릭터의 일반 포트레이트, 패배한 플레이어의 캐릭터의 다친 포트레이트를 표시합니다.
//        승리한 플레이어의 캐릭터가 가진 <승리 대사> 를 출력합니다.
// 설명 : 지정된 시간(미정) 동안 1P와 2P의 선택된 캐릭터의 포트레이트의 상태로 승패를 확인할 수 있습니다. 시간 초과시 "ContinueScene" 으로 씬을 전환합니다.
//        이동 키를 제외한 모든 입력을 받을 시 씬의 재생을 스킵 후 바로 "ContinueScene" 으로 씬을 전환합니다.
class ScreenFade;
class EngineGeneralSoundPlayer;
class ResultBackground;
class ResultScene : public Scene
{
private:	// member Var
	static ScreenFade* fadeObject_;
	float interTime_;
	EngineGeneralSoundPlayer* bgmPlayer_;

	bool isSceneChange_;

	ResultBackground* resultBackground1_;
	ResultBackground* resultBackground2_;
	ResultBackground* resultBackground3_;

public:
	static bool isResultSceneNeedtobeReset_;

public:		
	ResultScene(); // default constructer 디폴트 생성자
	~ResultScene(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ResultScene(const ResultScene& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ResultScene(ResultScene&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ResultScene& operator=(const ResultScene& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ResultScene& operator=(const ResultScene&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Loading() override;
	void SceneUpdateBefore() override;
	void SceneUpdate() override;
};

