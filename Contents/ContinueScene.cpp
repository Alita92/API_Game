#include "ContinueScene.h"
#include <EngineGeneralTime.h>
#include <EngineGeneralDebug.h>
#include <EngineGeneralSoundPlayer.h>
#include <EngineGeneralSound.h>
#include <SceneManager.h>
#include <EngineGeneralDirectory.h>
#include "ScreenFade.h"
#include <AssetManager.h>
#include "FightScene.h"
#include "ResultScene.h"
#include <ImageAsset.h>
#include "CharacterSelectScene.h"
#include <InputManager.h>
#include "ContinueKOCounter.h"
#include "ContinueLoserPortrait.h"
// Static Var
// Static Func

// constructer destructer
ScreenFade* ContinueScene::fadeObject_ = nullptr;


ContinueScene::ContinueScene()
	: bgmPlayer_(nullptr), interTime_(11.0f), isSceneChange_(false), interTimeToChangeScene_(0.0f), timeSwitchNum_(0), isBgmPlaying_(false)
{

}

ContinueScene::~ContinueScene()
{
}

ContinueScene::ContinueScene(ContinueScene&& _other) noexcept
	: bgmPlayer_(nullptr), interTime_(11.0f), isSceneChange_(false), interTimeToChangeScene_(0.0f), timeSwitchNum_(0), isBgmPlaying_(false)
{
}

//member Func
void ContinueScene::Loading()
{
	EngineGeneralDirectory pathFinder = EngineGeneralDirectory();
	pathFinder.MovetoParentFolder("API_Game");
	if (false == pathFinder.MovetoChildFolder("\\Resources\\"))
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}
	if (nullptr == AssetManager::GetAssetManager().FindImportedImage("ContinueBackground.bmp"))
	{
		AssetManager::GetAssetManager().ImportImage("ContinueBackground.bmp",
			pathFinder.GetRelativePath("ContinueBackground.bmp"));
		ImageAsset* continueCounter = AssetManager::GetAssetManager().ImportImage("ContinueCounter.bmp",
			pathFinder.GetRelativePath("ContinueCounter.bmp"));
		continueCounter->CutImage({ 576,576 });

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Continue.mp3", pathFinder.GetRelativePath("Continue.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ContinueYes.mp3", pathFinder.GetRelativePath("ContinueYes.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("CountBeep.wav", pathFinder.GetRelativePath("CountBeep.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count9.wav", pathFinder.GetRelativePath("RefereeNine.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count8.wav", pathFinder.GetRelativePath("RefereeEight.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count7.wav", pathFinder.GetRelativePath("RefereeSeven.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count6.wav", pathFinder.GetRelativePath("RefereeSix.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count5.wav", pathFinder.GetRelativePath("RefereeFive.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count4.wav", pathFinder.GetRelativePath("RefereeFour.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count3.wav", pathFinder.GetRelativePath("RefereeThree.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Count0.wav", pathFinder.GetRelativePath("RefereeZero.mp3"));

		bgmPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
		soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
	}

	fadeObject_ = CreateObject<ScreenFade>();
	continueKOCounter_ = CreateObject<ContinueKOCounter>();
	continueLoserPortrait_ = CreateObject<ContinueLoserPortrait>();

	bgmPlayer_->PlayCountReset(1);
	soundPlayer_->PlayCountReset(1);
	isBgmPlaying_ = false;
	isSceneChange_ = false;
	timeSwitchNum_ = 10;
	interTime_ = 11.0f;
	interTimeToChangeScene_ = 0.0f;


	// 하단은 디버깅용 키입니다. 릴리즈시 제거 요망
	if (false == InputManager::GetInputManager().IsButtonKey("DebugSkip"))
	{
		InputManager::GetInputManager().CreateInputKey("DebugSkip", '6');
	}
	
}

void ContinueScene::SceneUpdateBefore()
{
	continueLoserPortrait_->ChangePortrait(FightScene::loserName_);
}

void ContinueScene::SceneUpdate()
{
	if (false == isBgmPlaying_)
	{
		bgmPlayer_->PlayOverlap("Continue.mp3", 0);
		isBgmPlaying_ = true;
	}
	
	if (false == isSceneChange_)
	{
		interTime_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
	}
	

	if (true == InputManager::GetInputManager().IsButtonDown("P1_LP") || InputManager::GetInputManager().IsButtonDown("P1_MP") || InputManager::GetInputManager().IsButtonDown("P1_HP")
		|| InputManager::GetInputManager().IsButtonDown("P1_LK") || InputManager::GetInputManager().IsButtonDown("P1_MK") || InputManager::GetInputManager().IsButtonDown("P1_HK")
		|| InputManager::GetInputManager().IsButtonDown("P2_LP") || InputManager::GetInputManager().IsButtonDown("P2_MP") || InputManager::GetInputManager().IsButtonDown("P2_HP")
		|| InputManager::GetInputManager().IsButtonDown("P2_LK") || InputManager::GetInputManager().IsButtonDown("P2_MK") || InputManager::GetInputManager().IsButtonDown("P2_HK"))
	{
		switch (timeSwitchNum_)
		{
		case 10:
		{
			interTime_ = 10.0f;
		}
			break;
		case 9:
		{
			interTime_ = 9.0f;
		}
			break;
		case 8:
		{
			interTime_ = 8.0f;
		}
			break;
		case 7:
		{
			interTime_ = 7.0f;
		}
			break;
		case 6:
		{
			interTime_ = 6.0f;
		}
			break;
		case 5:
		{
			interTime_ = 5.0f;
		}
			break;
		case 4:
		{
			interTime_ = 4.0f;
		}
			break;
		case 3:
		{
			interTime_ = 3.0f;
		}
			break;
		case 2:
		{
			interTime_ = 2.0f;
		}
			break;
		case 1:
		{
			interTime_ = 1.0f;
		}
			break;
		case 0:
		{
			interTime_ = 0.0f;
		}
			break;
		default:
			break;
		}
	}

	if (true == InputManager::GetInputManager().IsButtonDown("Coin"))
	{
		timeSwitchNum_ = 9;
		interTime_ = 10.0f;
		continueKOCounter_->ChangeNumber("9");
		soundPlayer_->PlayCountReset(1);
		soundPlayer_->PlayOverlap("Count9.wav", 0);
		soundPlayer_->PlayCountReset(1);
		soundPlayer_->PlayOverlap("Start.wav", 0);
	}

	if (true == InputManager::GetInputManager().IsButtonDown("Start") && false == isSceneChange_)
	{
		bgmPlayer_->Stop();
		soundPlayer_->PlayCountReset(1);
		soundPlayer_->PlayOverlap("ContinueYes.mp3", 0);
		fadeObject_->FadeIn();
		isSceneChange_ = true;
	}

	if (true == isSceneChange_)
	{
		interTimeToChangeScene_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();

		if (2.0f <= interTimeToChangeScene_)
		{
			bgmPlayer_->Stop();
			CharacterSelectScene::isSelectSceneNeedtobeReset_ = true;
			FightScene::isFightSceneNeedtobeReset_ = true;
			ResultScene::isResultSceneNeedtobeReset_ = true;
			SceneManager::GetSceneManager().ChangeScene("CharaterSelectScene");
			return Reset();
		}
	}


	if (10.0f >= interTime_ && 10 == timeSwitchNum_)
	{
		soundPlayer_->PlayCountReset(1);
		soundPlayer_->PlayOverlap("Count9.wav", 0);

		// 나인
		timeSwitchNum_--;
	}
	if (9.0f >= interTime_ && 9 == timeSwitchNum_)
	{
		// 에잇
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("8");
		soundPlayer_->PlayOverlap("Count8.wav", 0);

		timeSwitchNum_--;
	}
	if (8.0f >= interTime_ && 8 == timeSwitchNum_)
	{
		// 세븐
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("7");
		soundPlayer_->PlayOverlap("Count7.wav", 0);
		timeSwitchNum_--;
	}
	if (7.0f >= interTime_ && 7 == timeSwitchNum_)
	{
		// 식스
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("6");
		soundPlayer_->PlayOverlap("Count6.wav", 0);
		timeSwitchNum_--;
	}
	if (6.0f >= interTime_ && 6 == timeSwitchNum_)
	{
		// 파이브
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("5");
		soundPlayer_->PlayOverlap("Count5.wav", 0);
		timeSwitchNum_--;
	}
	if (5.0f >= interTime_ && 5 == timeSwitchNum_)
	{
		// 포
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("4");
		soundPlayer_->PlayOverlap("Count4.wav", 0);
		timeSwitchNum_--;
	}
	if (4.0f >= interTime_ && 4 == timeSwitchNum_)
	{
		// 쓰리
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("3");
		soundPlayer_->PlayOverlap("Count3.wav", 0);
		timeSwitchNum_--;
	}
	if (3.0f >= interTime_ && 3 == timeSwitchNum_)
	{
		// 투
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("2");
		soundPlayer_->PlayOverlap("RefereeTwo.mp3", 0);
		timeSwitchNum_--;
	}
	if (2.0f >= interTime_ && 2 == timeSwitchNum_)
	{
		// 원
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("1");
		soundPlayer_->PlayOverlap("RefereeOne.mp3", 0);
		timeSwitchNum_--;
	}
	if (1.0f >= interTime_ && 1 == timeSwitchNum_)
	{
		soundPlayer_->PlayCountReset(1);
		continueKOCounter_->ChangeNumber("0");
		soundPlayer_->PlayOverlap("Count0.wav", 0);
		timeSwitchNum_--;
	}
	if (0 >= interTime_ && false == isGameOver_)
	{
		fadeObject_->FadeIn();
		isGameOver_ = true;
	}

	if (true == isGameOver_)
	{
		interTimeToChangeScene_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		if (3.0f < interTimeToChangeScene_)
		{
			bgmPlayer_->Stop();
			bgmPlayer_->PlayCountReset(1);
			soundPlayer_->PlayCountReset(1);
			isBgmPlaying_ = false;
			isSceneChange_ = false;
			timeSwitchNum_ = 10;
			interTime_ = 11.0f;
			interTimeToChangeScene_ = 0.0f;
			SceneManager::GetSceneManager().ChangeScene("EndScene");
		}
	}

	if (true == InputManager::GetInputManager().IsButtonDown("DebugSkip"))
	{
		bgmPlayer_->Stop();
		SceneManager::GetSceneManager().ChangeScene("EndScene");
	}
}