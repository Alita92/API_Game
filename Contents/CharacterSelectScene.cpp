#include "CharacterSelectScene.h"
#include <AssetManager.h>
#include <ImageAsset.h>
#include <EngineGeneralTime.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <EngineGeneralDirectory.h>
#include <EngineGeneralSound.h>
#include <EngineGeneralSoundPlayer.h>

#include "SelectCRT.h"
#include "SelectBackground.h"
#include "SelectionPortrait.h"
#include "Cursor.h"
#include "ScreenFade.h"
#include "SelectionFlag.h"
#include "SelectVSBackground.h"
#include "VSTypo.h"
#include "VSPortrait.h"
#include "VSName.h"
// Static Var
// Static Func


ScreenFade* CharacterSelectScene::fadeObject_ = nullptr;
std::string CharacterSelectScene::p1Selection_ = "";
std::string CharacterSelectScene::p2Selection_ = "";
bool CharacterSelectScene::isSelectSceneNeedtobeReset_ = false;


// constructer destructer
CharacterSelectScene::CharacterSelectScene()
	: p1SelectedCharacter_(""), p2SelectedCharacter_(""), interTime_(0.0f), isVSin_(false), isSceneChange_(false)
{

}

CharacterSelectScene::~CharacterSelectScene()
{
}

CharacterSelectScene::CharacterSelectScene(CharacterSelectScene&& _other) noexcept
	: p1SelectedCharacter_(""), p2SelectedCharacter_(""), interTime_(0.0f), isVSin_(false), isSceneChange_(false)
{
}

//member Func
void CharacterSelectScene::Loading()
{
	EngineGeneralDirectory pathFinder = EngineGeneralDirectory();
	pathFinder.MovetoParentFolder("API_Game");
	if (false == pathFinder.MovetoChildFolder("\\Resources\\"))
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	if (nullptr == AssetManager::GetAssetManager().FindImportedImage("SelectBackground.bmp"))
	{
		AssetManager::GetAssetManager().ImportImage("SelectBackground.bmp",
			pathFinder.GetRelativePath("SelectBackground.bmp"));
		AssetManager::GetAssetManager().ImportImage("SelectVSBackground.bmp",
			pathFinder.GetRelativePath("ResultBackground.bmp"));

		AssetManager::GetAssetManager().ImportImage("VSTypo.bmp",
			pathFinder.GetRelativePath("VSBackground.bmp"));
		ImageAsset* portraitSmall = AssetManager::GetAssetManager().ImportImage("CharacterPortraitSmall.bmp",
			pathFinder.GetRelativePath("CharacterPortraitSmall.bmp"));
		portraitSmall->CutImage({ 384, 288 });
		ImageAsset* cursor = AssetManager::GetAssetManager().ImportImage("Cursor.bmp",
			pathFinder.GetRelativePath("CursorResized.bmp"));
		cursor->CutImage({ 640, 374 });
		ImageAsset* flag = AssetManager::GetAssetManager().ImportImage("Flag.bmp",
			pathFinder.GetRelativePath("Flag.bmp"));
		flag->CutImage({ 192,192 });
		//ImageAsset* fadeScreen = AssetManager::GetAssetManager().ImportImage("ScreenFade.bmp", pathFinder.GetRelativePath("ScreenFade1.bmp"));
		ImageAsset* PortraitSprites = AssetManager::GetAssetManager().ImportImage("Portrait.bmp",
			pathFinder.GetRelativePath("portraitLNew.bmp"));
		PortraitSprites->CutImage({ 450,450 });
		ImageAsset* VSNameSprites = AssetManager::GetAssetManager().ImportImage("VSName.bmp",
			pathFinder.GetRelativePath("VSname.bmp"));
		VSNameSprites->CutImage({ 384,192 });

		bgmPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
		if (nullptr == bgmPlayer_)
		{
			EngineGeneralDebug::MsgBoxError("멤버 변수에 사운드 플레이어가 만들어지지 않았습니다.");
		}
		soundPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
		if (nullptr == soundPlayer_)
		{
			EngineGeneralDebug::MsgBoxError("멤버 변수에 사운드 플레이어가 만들어지지 않았습니다.");
		}

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("PlayerSelect.mp3", pathFinder.GetRelativePath("PlayerSelect.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("VS.mp3", pathFinder.GetRelativePath("VS.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("CursorMove.wav", pathFinder.GetRelativePath("CursorMove.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("CursorSelect.wav", pathFinder.GetRelativePath("CursorSelect.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("SelectPlane.wav", pathFinder.GetRelativePath("SelectPlane.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("SelectJapan.mp3", pathFinder.GetRelativePath("SelectJapan.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("SelectUSA.mp3", pathFinder.GetRelativePath("SelectUSA.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("SelectChina.mp3", pathFinder.GetRelativePath("SelectChina.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("SelectEngland.mp3", pathFinder.GetRelativePath("SelectEngland.mp3"));
	}
	CreateObject<SelectCRT>();
	soundPlayer_->PlayCountReset(3);
	bgmPlayer_->PlayCountReset(1);
	SelectBackground* selectBackground = CreateObject<SelectBackground>();
	Cursor* p1Cursor = CreateObject<Cursor>("p1Cursor");
	Cursor* p2Cursor = CreateObject<Cursor>("p2Cursor");

	SelectionPortrait* p1Portrait = CreateObject<SelectionPortrait>("p1Portrait");
	SelectionPortrait* p2Portrait = CreateObject<SelectionPortrait>("p2Portrait");

	SelectionFlag* p1Flag = CreateObject<SelectionFlag>("p1Flag");
	SelectionFlag* p2Flag = CreateObject<SelectionFlag>("p2Flag");

	fadeObject_ = CreateObject<ScreenFade>("ScreenFade1");

	vsBackground1_ = CreateObject<SelectVSBackground>();
	vsBackground2_ = CreateObject<SelectVSBackground>();
	vsBackground3_ = CreateObject<SelectVSBackground>();
	vsTypo_ = CreateObject<VSTypo>();

	p1VSPortrait_ = CreateObject<VSPortrait>();
	p2VSPortrait_ = CreateObject<VSPortrait>();
	p1VSName_ = CreateObject<VSName>();
	p2VSName_ = CreateObject<VSName>();

	p1Cursor->SetParentPortrait(p1Portrait);
	p2Cursor->SetParentPortrait(p2Portrait);
	p1Cursor->PlayerInit();
	p2Cursor->PlayerInit();
	p1Cursor->SetOtherCursor(p2Cursor);
	p2Cursor->SetOtherCursor(p1Cursor);
	p1Portrait->SetPos({ 156, 500 });
	p2Portrait->SetPos({ 996,500 });
	p1Flag->PlayerInit();
	p2Flag->PlayerInit();
	SetP1Cursor(p1Cursor);
	SetP2Cursor(p2Cursor);
	p1Cursor->SetParentFlag(p1Flag);
	p2Cursor->SetParentFlag(p2Flag);
	p1VSPortrait_->SetPos({ 200,275 });
	p2VSPortrait_->SetPos({ 952,275 });
	p1VSName_->SetPos({ 200, 500 });
	p2VSName_->SetPos({ 952,500 });
	vsBackground1_->SetPos({ 576,336 });
	vsBackground2_->SetPos({ 576,336 + 672 });
	vsBackground3_->SetPos({ 576, 336 + 672 + 672 });
	vsBackground1_->Off();
	vsBackground2_->Off();
	vsBackground3_->Off();
	vsTypo_->Off();
	p1VSPortrait_->Off();
	p2VSPortrait_->Off();
	p1VSName_->Off();
	p2VSName_->Off();

	// 하단은 디버깅용 키입니다. 릴리즈시 제거 요망
//	if (false == InputManager::GetInputManager().IsButtonKey("DebugSkip"))
//	{
//		InputManager::GetInputManager().CreateInputKey("DebugSkip", '6');
//	}
}

void CharacterSelectScene::SceneUpdateBefore()
{

	bgmPlayer_->PlayAlone("PlayerSelect.mp3", 0);
	if (true == isSelectSceneNeedtobeReset_)
	{
		p1SelectedCharacter_ = "";
		p2SelectedCharacter_ = "";

		isVSin_ = false;
		interTime_ = 0.0f;
		isSceneChange_ = false;
		fadeObject_->FadeOut();
		isFadein_ = false;
		isSelectSceneNeedtobeReset_ = false;
		Reset();
	}
}


void CharacterSelectScene::SceneUpdate()
{

	if (InputManager::GetInputManager().IsButtonDown("Debug"))
	{
		Reset();
	}

	if (true == p1_->isReady_ && true == p2_->isReady_) // p1 p2 캐릭터 선택 완료했다면...
	{
		interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		p1SelectedCharacter_ = p1_->CharacterSelected();
		p2SelectedCharacter_ = p2_->CharacterSelected();

		if (0.8f < interTime_)
		{
			soundPlayer_->PlayOverlap("SelectPlane.wav", 0);
		}

		if (1.5f < interTime_ && false == isFadein_)
		{
			if ("Ken" == p2SelectedCharacter_ || "Guile" == p2SelectedCharacter_ || "Boxer" == p2SelectedCharacter_)
			{
				soundPlayer_->PlayCountReset(1);
				soundPlayer_->PlayOverlap("SelectUSA.mp3", 0);
			}
			if ("Ryu" == p2SelectedCharacter_ || "EHonda" == p2SelectedCharacter_)
			{
				soundPlayer_->PlayCountReset(1);
				soundPlayer_->PlayOverlap("SelectJapan.mp3", 0);
			}
			if ("ChunLi" == p2SelectedCharacter_ || "FeiLong" == p2SelectedCharacter_)
			{
				soundPlayer_->PlayCountReset(1);
				soundPlayer_->PlayOverlap("SelectChina.mp3", 0);
			}
			if ("Cammy" == p2SelectedCharacter_)
			{
				soundPlayer_->PlayCountReset(1);
				soundPlayer_->PlayOverlap("SelectEngland.mp3", 0);
			}

			fadeObject_->FadeIn();
			isFadein_ = true;
		}

		if (3.0f < interTime_ && false == isVSin_)
		{
			bgmPlayer_->Stop();
			bgmPlayer_->PlayCountReset(1);
			bgmPlayer_->PlayAlone("VS.mp3", 0);
			vsBackground1_->On();
			vsBackground2_->On();
			vsBackground3_->On();
			vsTypo_->On();
			p1VSPortrait_->SetPortrait("P1Portrait" + p1SelectedCharacter_);
			p2VSPortrait_->SetPortrait("P2Portrait" + p2SelectedCharacter_);
			p1VSName_->SetVSName(p1SelectedCharacter_);
			p2VSName_->SetVSName(p2SelectedCharacter_);
			p1Selection_ = p1SelectedCharacter_;
			p2Selection_ = p2SelectedCharacter_;
			p1VSPortrait_->On();
			p2VSPortrait_->On();
			p1VSName_->On();
			p2VSName_->On();
			fadeObject_->FadeOut();
			isVSin_ = true;
			interTime_ = 0.0f;
		}
	}

	if (true == isVSin_ && false == isSceneChange_)
	{
		if (true == InputManager::GetInputManager().IsButtonDown("P1_LP") || InputManager::GetInputManager().IsButtonDown("P1_MP") || InputManager::GetInputManager().IsButtonDown("P1_HP")
			|| InputManager::GetInputManager().IsButtonDown("P1_LK") || InputManager::GetInputManager().IsButtonDown("P1_MK") || InputManager::GetInputManager().IsButtonDown("P1_HK")
			|| InputManager::GetInputManager().IsButtonDown("P2_LP") || InputManager::GetInputManager().IsButtonDown("P2_MP") || InputManager::GetInputManager().IsButtonDown("P2_HP")
			|| InputManager::GetInputManager().IsButtonDown("P2_LK") || InputManager::GetInputManager().IsButtonDown("P2_MK") || InputManager::GetInputManager().IsButtonDown("P2_HK"))
		{
			interTime_ = 0.0f;
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
			SceneManager::GetSceneManager().ChangeScene("FightScene");
		}
	}

	//if (true == InputManager::GetInputManager().IsButtonDown("DebugSkip"))
	//{
	//	bgmPlayer_->Stop();
	//	p1SelectedCharacter_ = "Cammy";
	//	p2SelectedCharacter_ = "ChunLi";
	//	p1Selection_ = "Cammy";
	//	p2Selection_ = "ChunLi";
	//	SceneManager::GetSceneManager().ChangeScene("FightScene");
	//}
}
