#include "ResultScene.h"
#include "ResultBackground.h"
#include "WinnerTexth.h"
#include "ResultPortrait.h"
#include "FightScene.h"
#include <AssetManager.h>
#include <ImageAsset.h>
#include <EngineGeneralDirectory.h>
#include <EngineGeneralSound.h>
#include <EngineGeneralSoundPlayer.h>
#include <EngineGeneralTime.h>
#include <InputManager.h>
#include "ScreenFade.h"
#include "SelectCRT.h"
#include <SceneManager.h>

// Static Var
// Static Func
ScreenFade* ResultScene::fadeObject_ = nullptr;
bool ResultScene::isResultSceneNeedtobeReset_ = false;

// constructer destructer
ResultScene::ResultScene()
	:interTime_(0.0f), bgmPlayer_(nullptr), isSceneChange_(false)
{
}

ResultScene::~ResultScene()
{
}

ResultScene::ResultScene(ResultScene&& _other) noexcept
	:interTime_(0.0f), bgmPlayer_(nullptr), isSceneChange_(false)
{
}

//member Func
void ResultScene::Loading()
{

	EngineGeneralDirectory pathFinder = EngineGeneralDirectory();
	pathFinder.MovetoParentFolder("API_Game");
	if (false == pathFinder.MovetoChildFolder("\\Resources\\"))
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	if (nullptr == AssetManager::GetAssetManager().FindImportedImage("RyuWinText.bmp"))
	{
		AssetManager::GetAssetManager().ImportImage("ResultBackground.bmp",
			pathFinder.GetRelativePath("ResultBackground.bmp"));
		AssetManager::GetAssetManager().ImportImage("RyuWinText.bmp",
			pathFinder.GetRelativePath("RyuWinText.bmp"));
		AssetManager::GetAssetManager().ImportImage("KenWinText.bmp",
			pathFinder.GetRelativePath("KenWinText.bmp"));
		AssetManager::GetAssetManager().ImportImage("ChunLiWinText.bmp",
			pathFinder.GetRelativePath("ChunLiWinText.bmp"));
		AssetManager::GetAssetManager().ImportImage("CammyWinText.bmp",
			pathFinder.GetRelativePath("CammyWinText.bmp"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Result.mp3", pathFinder.GetRelativePath("Result.mp3"));


		bgmPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	
	}

	CreateObject<SelectCRT>();
	bgmPlayer_->PlayCountReset(1);
	CreateObject<WinnerTexth>();
	resultBackground1_ = CreateObject<ResultBackground>();
	resultBackground2_ = CreateObject<ResultBackground>();
	resultBackground3_ = CreateObject<ResultBackground>();
	resultBackground1_->SetPos({ 576,336 });
	resultBackground2_->SetPos({ 576,336 + 672 });
	resultBackground3_->SetPos({ 576, 336 + 672 + 672 });
	ResultPortrait* portrait1P = CreateObject<ResultPortrait>("P1");
	ResultPortrait* portrait2P = CreateObject<ResultPortrait>("P2");
	portrait1P->SetPos({280,316});
	portrait2P->SetPos({872,316});
	interTime_ = 0.0f;
	fadeObject_ = CreateObject<ScreenFade>("ScreenFade");
	isSceneChange_ = false;

	// 하단은 디버깅용 키입니다. 릴리즈시 제거 요망
	if (false == InputManager::GetInputManager().IsButtonKey("DebugSkip"))
	{
		InputManager::GetInputManager().CreateInputKey("DebugSkip", '6');
	}
}

void ResultScene::SceneUpdateBefore()
{
	if (true == ResultScene::isResultSceneNeedtobeReset_)
	{
		interTime_ = 0.0f;
		
		isSceneChange_ = false;

		Reset();
		isResultSceneNeedtobeReset_ = false;
	}
}

void ResultScene::SceneUpdate()
{
	bgmPlayer_->PlayOverlap("Result.mp3", 0);
	if (false == isSceneChange_)
	{
		if (true == InputManager::GetInputManager().IsButtonDown("P1_LP") || InputManager::GetInputManager().IsButtonDown("P1_MP") || InputManager::GetInputManager().IsButtonDown("P1_HP")
			|| InputManager::GetInputManager().IsButtonDown("P1_LK") || InputManager::GetInputManager().IsButtonDown("P1_MK") || InputManager::GetInputManager().IsButtonDown("P1_HK")
			|| InputManager::GetInputManager().IsButtonDown("P2_LP") || InputManager::GetInputManager().IsButtonDown("P2_MP") || InputManager::GetInputManager().IsButtonDown("P2_HP")
			|| InputManager::GetInputManager().IsButtonDown("P2_LK") || InputManager::GetInputManager().IsButtonDown("P2_MK") || InputManager::GetInputManager().IsButtonDown("P2_HK"))
		{
			fadeObject_->FadeIn();
			isSceneChange_ = true;
		}
	}

	if (true == isSceneChange_)
	{
		interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		if (3.0f < interTime_)
		{
			bgmPlayer_->Stop();
			SceneManager::GetSceneManager().ChangeScene("ContinueScene");
		}
	}

	if (true == InputManager::GetInputManager().IsButtonDown("DebugSkip"))
	{
		bgmPlayer_->Stop();
		bgmPlayer_->PlayCountReset(1);
		interTime_ = 0.0f;
		isSceneChange_ = false;
		SceneManager::GetSceneManager().ChangeScene("ContinueScene");
	}
	
}