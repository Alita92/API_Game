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

void GameLoop() // ������ ��� ������Ʈ�� �����ϴ�. �⺻ �޼��� ������ Loop() �� �Լ� �����ͷ� ���ϴ�.
{
	SceneManager::GetSceneManager().Update();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// �޸� �� üũ
	
	EngineGeneralDebug::CheckMemoryLeak();

	EngineGeneralSound::GetEngineGeneralSound().SoundInitialize();

	// ������ ȭ���� �������� ������ OS�� ���� ��, �� ������ �´� ȭ���� �ε� ��û�մϴ�.
	EngineScreen::GetScreen().CreateScreenClass(hInstance, "GameScreen");
	EngineScreen::GetScreen().CreateScreen("Super Street Fighter 2 : The New Challengers", { 1152, 672 }, { 200,0 });
	
	// ����� ���� �� ������ ������Ʈ���� ������ݴϴ�. (Ÿ��Ʋ / ĳ���� ���� / VS���� / ������ / ��� / ��Ƽ�� �� 6�� ���Դϴ�)
	SceneManager::GetSceneManager().CreateScene<TitleScene>("TitleScene");
	SceneManager::GetSceneManager().CreateScene<CharacterSelectScene>("CharaterSelectScene");
	SceneManager::GetSceneManager().CreateScene<FightScene>("FightScene");
	SceneManager::GetSceneManager().CreateScene<ResultScene>("ResultScene");
	SceneManager::GetSceneManager().CreateScene<ContinueScene>("ContinueScene");
	SceneManager::GetSceneManager().CreateScene<EndScene>("EndScene");
	 // ó�� ȭ�鿡 ���� ���� �������ݴϴ�. (Ÿ��Ʋ ���Դϴ�)
	SceneManager::GetSceneManager().ChangeScene("TitleScene");
	
	 // �Է� ������ ���� ������ ������ �����մϴ�.
	EngineScreen::GetScreen().ScreenLoop(GameLoop);
	
	// ������ ������(������ ����Ǹ�) ���� üũ�� �����ݴϴ�.
	AppCleaner();
}


