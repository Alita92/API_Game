#include "TitleScene.h"
#include "TitleLogo.h"
#include "TitleTypoSprites.h"
#include <ImageAsset.h>
#include <AssetManager.h>
#include <EngineGeneralDirectory.h>
#include <InputManager.h>
#include <EngineGeneralSound.h>
#include <EngineGeneralSoundPlayer.h>
#include "ScreenFade.h"
#include <Windows.h>
#include <SceneManager.h>
#include <EngineGeneralTime.h>
#include <EngineScreen.h>


ScreenFade* TitleScene::fadeObject_ = nullptr;

TitleScene::TitleScene()
	: interTime_(0.0f)
{
}

TitleScene::~TitleScene()
{
}

TitleScene::TitleScene(TitleScene&& _other) noexcept
	: interTime_(0.0f)
{
}

void TitleScene::Loading() // ���� Ȱ��ȭ�� �� �ҷ��� ������Ʈ���� ����ϴ�.
{
	EngineGeneralDirectory pathFinder = EngineGeneralDirectory();
	pathFinder.MovetoParentFolder("API_Game");
	if (false == pathFinder.MovetoChildFolder("\\Resources\\"))
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	AssetManager::GetAssetManager().ImportImage("TitleLogo.bmp",
		pathFinder.GetRelativePath("TitleLogo.bmp"));
	ImageAsset* titleAnimation = AssetManager::GetAssetManager().ImportImage("TitleAnimation.bmp",
		pathFinder.GetRelativePath("TitleAnimation.bmp"));
	titleAnimation->CutImage({ 1152, 672 });
	AssetManager::GetAssetManager().ImportImage("InsertCoin.bmp",
		pathFinder.GetRelativePath("InsertCoin.bmp"));
	AssetManager::GetAssetManager().ImportImage("PushStart.bmp",
		pathFinder.GetRelativePath("PushStart.bmp"));
	AssetManager::GetAssetManager().ImportImage("CRT.bmp", pathFinder.GetRelativePath("CRT.bmp"));
	EngineGeneralSound::GetEngineGeneralSound().ImportSound("TitleMusic.mp3", pathFinder.GetRelativePath("TitleMusic.mp3"));
	EngineGeneralSound::GetEngineGeneralSound().ImportSound("Start.wav", pathFinder.GetRelativePath("Start.wav"));
	ImageAsset* fadeScreen = AssetManager::GetAssetManager().ImportImage("ScreenFade.bmp", pathFinder.GetRelativePath("ScreenFade1.bmp"));
	soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	soundPlayer_->PlayCountReset(1);
	CreateObject<TitleLogo>();
	typo_ = CreateObject<TitleTypoSprites>();
	fadeObject_ = CreateObject<ScreenFade>("ScreenFadeTitle");

	EngineScreen::GetScreen().GetMoviePlayer()->Play(L"..\\Resources\\Intro.wmv");

	if (false == InputManager::GetInputManager().IsButtonKey("Start"))
	{
		InputManager::GetInputManager().CreateInputKey("Start", VK_RETURN);
		InputManager::GetInputManager().CreateInputKey("Coin", '5');
		// �ϴ��� ������ Ű�Դϴ�. ������� ���� ���
		//InputManager::GetInputManager().CreateInputKey("DebugSkip", '6');
		InputManager::GetInputManager().CreateInputKey("P1_AIToggle", '1');
		InputManager::GetInputManager().CreateInputKey("P2_AIToggle", '2');
	}
}

void TitleScene::SceneUpdate() // ����� �����Ӱ� ����, �ð� ���� �ֱ������� ������Ʈ�մϴ�.
{

	if (true == InputManager::GetInputManager().IsButtonDown("Coin") && false == isStart_)
	{
		EngineScreen::GetScreen().GetMoviePlayer()->Stop();
		soundPlayer_->PlayCountReset(1);
		soundPlayer_->PlayOverlap("Start.wav",0);
		typo_->ChangeSprite();
		isCoin_ = true;
	}

	if (true == InputManager::GetInputManager().IsButtonDown("Start") && false == isStart_ && true == isCoin_)
	{
		fadeObject_->FadeIn();
		isStart_ = true;
	}

	if (true == isStart_)
	{
		interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		
		if (2.5f < interTime_)
		{
			SceneManager::GetSceneManager().ChangeScene("CharaterSelectScene");
		}
	}

	if (true == InputManager::GetInputManager().IsButtonDown("DebugSkip"))
	{
		//EngineScreen::GetScreen().GetMoviePlayer()->Stop();
		//SceneManager::GetSceneManager().ChangeScene("CharaterSelectScene");
	}
}