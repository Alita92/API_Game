#include "EndScene.h"
#include "End.h"
#include "ScreenFade.h"
#include <AssetManager.h>
#include <EngineGeneralDirectory.h>
#include <EngineGeneralSoundPlayer.h>
#include <EngineGeneralSound.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <EngineGeneralTime.h>

ScreenFade* EndScene::fadeObject_ = nullptr;

EndScene::EndScene()
	: interTime_(0.0f), bgmPlayer_(nullptr)
{
}

EndScene::~EndScene()
{
}

EndScene::EndScene(EndScene&& _other) noexcept
	: interTime_(0.0f), bgmPlayer_(nullptr)
{
}



void EndScene::Loading()
{
	EngineGeneralDirectory pathFinder = EngineGeneralDirectory();
	pathFinder.MovetoParentFolder("API_Game");
	if (false == pathFinder.MovetoChildFolder("\\Resources\\"))
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	fadeObject_ = CreateObject<ScreenFade>();

	AssetManager::GetAssetManager().ImportImage("End.bmp",
		pathFinder.GetRelativePath("End.bmp"));
	CreateObject<End>();
	EngineGeneralSound::GetEngineGeneralSound().ImportSound("End.mp3", pathFinder.GetRelativePath("End.mp3"));
	EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeChampion.mp3", pathFinder.GetRelativePath("RefereeChampion.mp3"));
	bgmPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	bgmPlayer_->PlayCountReset(1);
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	soundPlayer_->PlayCountReset(1);
	// 하단은 디버깅용 키입니다. 릴리즈시 제거 요망
	if (false == InputManager::GetInputManager().IsButtonKey("DebugSkip"))
	{
		InputManager::GetInputManager().CreateInputKey("DebugSkip", '6');
	}
}

void EndScene::SceneUpdate()
{
	interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

	bgmPlayer_->PlayAlone("End.mp3");

	if (1.5f < interTime_)
	{
		soundPlayer_->PlayOverlap("RefereeChampion.mp3",0);
	}


	if (true == InputManager::GetInputManager().IsButtonDown("DebugSkip"))
	{
		bgmPlayer_->Stop();
		bgmPlayer_->PlayCountReset(1);
		SceneManager::GetSceneManager().ChangeScene("TitleScene");
	}
}