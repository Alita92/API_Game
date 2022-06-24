// Tier WinAPI
#include <iostream>
#include <Windows.h>

// Tier 0
#include <EngineGeneralDebug.h>
#include <EngineGeneralMath.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralSound.h>

// Tier 1
#include <EngineScreen.h>
#include <SceneManager.h>
#include <Object.h>
#include <InputManager.h>

// Tier 2
#include <TitleScene.h>
#include <CharacterSelectScene.h>
#include <FightScene.h>
#include <ResultScene.h>
#include <ContinueScene.h>
#include <EndScene.h>

#include "AppCleaner.h"

void GameLoop() // 게임의 기반 업데이트를 돌립니다. 기본 메세지 루프인 Loop() 의 함수 포인터로 들어갑니다.
{
	SceneManager::GetSceneManager().Update();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 메모리 릭 체크
	
	EngineGeneralDebug::CheckMemoryLeak();

	EngineGeneralSound::GetEngineGeneralSound().SoundInitialize();

	// 윈도우 화면의 기지정된 컨셉을 OS에 전달 후, 그 컨셉에 맞는 화면을 로딩 요청합니다.
	EngineScreen::GetScreen().CreateScreenClass(hInstance, "GameScreen");
	EngineScreen::GetScreen().CreateScreen("Super Street Fighter 2 : The New Challengers", { 1152, 672 }, { 200,0 });
	
	// 사용할 씬과 씬 내부의 오브젝트들을 만들어줍니다. (타이틀 / 캐릭터 선택 / VS데모 / 본게임 / 결과 / 컨티뉴 총 6개 씬입니다)
	SceneManager::GetSceneManager().CreateScene<TitleScene>("TitleScene");
	SceneManager::GetSceneManager().CreateScene<CharacterSelectScene>("CharaterSelectScene");
	SceneManager::GetSceneManager().CreateScene<FightScene>("FightScene");
	SceneManager::GetSceneManager().CreateScene<ResultScene>("ResultScene");
	SceneManager::GetSceneManager().CreateScene<ContinueScene>("ContinueScene");
	SceneManager::GetSceneManager().CreateScene<EndScene>("EndScene");
	 // 처음 화면에 나올 씬을 선택해줍니다. (타이틀 씬입니다)
	SceneManager::GetSceneManager().ChangeScene("TitleScene");
	
	 // 입력 유무에 따른 루프를 돌리기 시작합니다.
	EngineScreen::GetScreen().ScreenLoop(GameLoop);
	
	// 루프가 끝나면(게임이 종료되면) 릭을 체크해 지워줍니다.
	AppCleaner();
}


