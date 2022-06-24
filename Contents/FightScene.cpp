#include "FightScene.h"
#include <SpriteRenderer.h>
#include <AssetManager.h>
#include <ImageAsset.h>
#include <EngineGeneralDirectory.h>
#include <EngineGeneralTime.h>
#include <SceneManager.h>
#include <EngineGeneralSound.h>
#include <EngineGeneralSoundPlayer.h>
#include <Scene.h>

#include "SelectCRT.h"
#include "CharacterSelectScene.h"
#include "CharacterHandle.h"
#include "Ryu.h"
#include "Projectile.h"
#include "Ken.h"
#include "ChunLi.h"
#include "Cammy.h"

#include "StageRyu.h"
#include "StageRyuMid.h"
#include "StageRyuBack.h"
#include "StageGuileFore.h"
#include "StageGuileMid.h"
#include "StageGuileBack.h"
#include "StageGuileFog.h"
#include "StageGuilePeople.h"
#include "StageChunLiFore.h"
#include "StageChunLiBack.h"
#include "StageCammy.h"
#include "StageCammyBack.h"
#include "Shadow.h"

#include "HitPointGauge.h"
#include "HPbar.h"
#include "Winpoint.h"
#include "FightEffects.h"
#include "HitPointName.h"

#include "TimeCounter.h"
#include "RefereeText.h"
#include "ScreenFade.h"

std::vector<float4> FightScene::DebugPos;

ScreenFade* FightScene::fadeObject_ = nullptr;
std::string FightScene::whoWon_ = "";
std::string FightScene::loserName_ = "";
std::string FightScene::winnerName_ = "";
bool FightScene::isFightSceneNeedtobeReset_ = false;
int FightScene::p1WinPoint_ = 0;
int FightScene::p2WinPoint_ = 0;


FightScene::FightScene()
	: interTimeA_(0.0f), p1HPbar_(nullptr), p2HPbar_(nullptr), p1_(nullptr), p2_(nullptr), refereeTextHandle_(nullptr), timeCounterOne_(nullptr),
	timeCounterTen_(nullptr), interTimeB_(0.0f), roundCount_(0), isRoundStart_(false), isCharacterLoaded_(false),
	soundRoundOn_(false), soundNumberOn_(false), soundFightOn_(false)
{

}

FightScene::~FightScene()
{
}

FightScene::FightScene(FightScene&& _other) noexcept
	: interTimeA_(0.0f), p1HPbar_(nullptr), p2HPbar_(nullptr), p1_(nullptr), p2_(nullptr), refereeTextHandle_(nullptr), timeCounterOne_(nullptr),
	timeCounterTen_(nullptr), interTimeB_(0.0f), roundCount_(0), isRoundStart_(false),  isCharacterLoaded_(false),
	soundRoundOn_(false), soundNumberOn_(false), soundFightOn_(false)
{
}

void FightScene::Loading()
{
	EngineGeneralDirectory pathFinder = EngineGeneralDirectory();
	pathFinder.MovetoParentFolder("API_Game");
	if (false == pathFinder.MovetoChildFolder("\\Resources\\"))
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	if (false == InputManager::GetInputManager().IsButtonKey("TestKey"))
	{
		InputManager::GetInputManager().CreateInputKey("TestKey", 'C');
	}

	if (nullptr == AssetManager::GetAssetManager().FindImportedImage("FightingEffects.bmp"))
	{
		ImageAsset* fightingEffects = AssetManager::GetAssetManager().ImportImage("FightingEffects.bmp",
			pathFinder.GetRelativePath("FightingEffects.bmp"));
		fightingEffects->CutImage({ 128,128 });
		ImageAsset* ryuSprites = AssetManager::GetAssetManager().ImportImage("SpritesRyu.bmp",
			pathFinder.GetRelativePath("SpritesRyu2.bmp"));
		ryuSprites->CutImage({ 768, 576 });
		ImageAsset* kenSprites = AssetManager::GetAssetManager().ImportImage("SpritesKen.bmp",
			pathFinder.GetRelativePath("SpritesKen2.bmp"));
		kenSprites->CutImage({ 768, 576 });
		ImageAsset* chunLiSprites = AssetManager::GetAssetManager().ImportImage("SpritesChunLi.bmp",
			pathFinder.GetRelativePath("SpritesChunli2.bmp"));
		chunLiSprites->CutImage({ 768,576 });
		ImageAsset* cammySprites = AssetManager::GetAssetManager().ImportImage("SpritesCammy.bmp",
			pathFinder.GetRelativePath("SpritesCammy2.bmp"));
		cammySprites->CutImage({ 768,576 });

		AssetManager::GetAssetManager().ImportImage("Shadow.bmp", pathFinder.GetRelativePath("Shadow.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageRyuFore.bmp",
			pathFinder.GetRelativePath("StageRyuFore1.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageRyuMid.bmp",
			pathFinder.GetRelativePath("StageRyuMid.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageRyuFog.bmp",
			pathFinder.GetRelativePath("StageRyuFog.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageGuileFore.bmp",
			pathFinder.GetRelativePath("StageGuileFore.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageGuileMid.bmp",
			pathFinder.GetRelativePath("StageGuileMid.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageGuileFog.bmp",
			pathFinder.GetRelativePath("StageGuileFog.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageGuileBack.bmp",
			pathFinder.GetRelativePath("StageGuileBack.bmp"));

		ImageAsset* stageGuilePeople = AssetManager::GetAssetManager().ImportImage("StageGuilePeople.bmp",
			pathFinder.GetRelativePath("StageGuilePeople.bmp"));
		stageGuilePeople->CutImage({ 410,307 });

		AssetManager::GetAssetManager().ImportImage("StageChunLiBack.bmp",
			pathFinder.GetRelativePath("StageChunLiBack.bmp"));
		AssetManager::GetAssetManager().ImportImage("StageChunLiFore.bmp",
			pathFinder.GetRelativePath("StageChunLiFore.bmp"));

		ImageAsset* stageCammy = AssetManager::GetAssetManager().ImportImage("StageCammy.bmp",
			pathFinder.GetRelativePath("StageCammy.bmp"));
		stageCammy->CutImage({ 2750, 940 });
		ImageAsset* stageCammyBack = AssetManager::GetAssetManager().ImportImage("StageCammyBack.bmp",
			pathFinder.GetRelativePath("StageCammyBack.bmp"));


		AssetManager::GetAssetManager().ImportImage("HitPointGauge.bmp", pathFinder.GetRelativePath("HitPointGauge.bmp"));
		AssetManager::GetAssetManager().ImportImage("HPbar.bmp", pathFinder.GetRelativePath("HPbar.bmp"));
		AssetManager::GetAssetManager().ImportImage("Winpoint.bmp", pathFinder.GetRelativePath("Winpoint.bmp"));
		ImageAsset* fightName = AssetManager::GetAssetManager().ImportImage("FightName.bmp", pathFinder.GetRelativePath("FightName.bmp"));
		fightName->CutImage({256,128});
		AssetManager::GetAssetManager().ImportImage("R1.bmp", pathFinder.GetRelativePath("R1.bmp"));
		AssetManager::GetAssetManager().ImportImage("R2.bmp", pathFinder.GetRelativePath("R2.bmp"));
		AssetManager::GetAssetManager().ImportImage("R3.bmp", pathFinder.GetRelativePath("R3.bmp"));
		AssetManager::GetAssetManager().ImportImage("Fight.bmp", pathFinder.GetRelativePath("Fight.bmp"));
		AssetManager::GetAssetManager().ImportImage("Combo.bmp", pathFinder.GetRelativePath("Combo.bmp"));
		AssetManager::GetAssetManager().ImportImage("Reversal.bmp", pathFinder.GetRelativePath("Reversal.bmp"));
		AssetManager::GetAssetManager().ImportImage("RyuWinSign.bmp", pathFinder.GetRelativePath("WinSign.bmp"));
		AssetManager::GetAssetManager().ImportImage("KenWinSign.bmp", pathFinder.GetRelativePath("WinSignKen.bmp"));
		AssetManager::GetAssetManager().ImportImage("ChunLiWinSign.bmp", pathFinder.GetRelativePath("WinSignChunLi.bmp"));
		AssetManager::GetAssetManager().ImportImage("CammyWinSign.bmp", pathFinder.GetRelativePath("WinSignCammy.bmp"));


		ImageAsset* newNumber = AssetManager::GetAssetManager().ImportImage("Numbers.bmp", pathFinder.GetRelativePath("Numbers.bmp"));
		newNumber->CutImage({ 96, 96 });


		bgmPlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
		if (nullptr == bgmPlayer_)
		{
			EngineGeneralDebug::MsgBoxError("멤버 변수에 사운드 플레이어가 만들어지지 않았습니다.");
		}

		refereePlayer_ = EngineGeneralSound::GetEngineGeneralSound().CreateSoundPlayer();
		if (nullptr == refereePlayer_)
		{
			EngineGeneralDebug::MsgBoxError("멤버 변수에 사운드 플레이어가 만들어지지 않았습니다.");
		}
		refereePlayer_->PlayCountReset(1);

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeRyu", pathFinder.GetRelativePath("RyuStage.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeRyuLast", pathFinder.GetRelativePath("RyuStageLast.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeGuile", pathFinder.GetRelativePath("GuileStage.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeGuileLast", pathFinder.GetRelativePath("GuileStageLast.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeChunLi", pathFinder.GetRelativePath("ChunLiStage.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeChunLiLast", pathFinder.GetRelativePath("ChunLiStageLast.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeCammy", pathFinder.GetRelativePath("CammyStage.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ThemeCammyLast", pathFinder.GetRelativePath("CammyStageLast.mp3"));

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeRound.mp3", pathFinder.GetRelativePath("RefereeRound.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeOne.mp3", pathFinder.GetRelativePath("RefereeOne.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeTwo.mp3", pathFinder.GetRelativePath("RefereeTwo.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeFinal.mp3", pathFinder.GetRelativePath("RefereeFinal.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeFight.mp3", pathFinder.GetRelativePath("RefereeFight.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeWin.mp3", pathFinder.GetRelativePath("RefereeWin.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereePerfect.mp3", pathFinder.GetRelativePath("RefereePerfect.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeRyu.mp3", pathFinder.GetRelativePath("RefereeRyu.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeKen.mp3", pathFinder.GetRelativePath("RefereeKen.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeChunLi.mp3", pathFinder.GetRelativePath("RefereeChunLi.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("RefereeCammy.mp3", pathFinder.GetRelativePath("RefereeCammy.mp3"));

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("LAttack.wav", pathFinder.GetRelativePath("LAttack.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("MAttack.wav", pathFinder.GetRelativePath("MAttack.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("HAttack.wav", pathFinder.GetRelativePath("HAttack.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("LHit.wav", pathFinder.GetRelativePath("LHit.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("MHit.wav", pathFinder.GetRelativePath("MHit.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("HHit.wav", pathFinder.GetRelativePath("HHit.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("CriticalHit.wav", pathFinder.GetRelativePath("CriticalHit.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Block.wav", pathFinder.GetRelativePath("Block.wav"));

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Hittheground.wav", pathFinder.GetRelativePath("Hittheground.wav"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ryu_Hadouken.mp3", pathFinder.GetRelativePath("Ryu_Hadouken.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ryu_Shoryuken.mp3", pathFinder.GetRelativePath("Ryu_Shoryuken.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ryu_Tatsumaki.mp3", pathFinder.GetRelativePath("Ryu_Tatsumaki.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ryu_Throw.mp3", pathFinder.GetRelativePath("Ryu_Throw.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ryu_KO.mp3", pathFinder.GetRelativePath("Ryu_KO.mp3"));

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ken_Hadouken.mp3", pathFinder.GetRelativePath("Ken_Hadouken.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ken_Shoryuken.mp3", pathFinder.GetRelativePath("Ken_Shoryuken.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ken_Tatsumaki.mp3", pathFinder.GetRelativePath("Ken_Tatsumaki.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ken_Throw.mp3", pathFinder.GetRelativePath("Ken_Throw.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Ken_KO.mp3", pathFinder.GetRelativePath("Ken_KO.mp3"));

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ChunLi_Kikoken.mp3", pathFinder.GetRelativePath("ChunLi_Kikoken.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ChunLi_SpinBirdKick.mp3", pathFinder.GetRelativePath("ChunLi_SpinBirdKick.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ChunLi_Hakuretsukyaku.mp3", pathFinder.GetRelativePath("ChunLi_Hakuretsukyaku.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ChunLi_Win.mp3", pathFinder.GetRelativePath("ChunLi_Win.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ChunLi_Throw.mp3", pathFinder.GetRelativePath("ChunLi_Throw.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("ChunLi_KO.mp3", pathFinder.GetRelativePath("ChunLi_KO.mp3"));

		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Cammy_SpiralArrow.mp3", pathFinder.GetRelativePath("Cammy_SpiralArrow.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Cammy_CannonSpike.mp3", pathFinder.GetRelativePath("Cammy_CannonSpike.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Cammy_Throw.mp3", pathFinder.GetRelativePath("Cammy_Throw.mp3"));
		EngineGeneralSound::GetEngineGeneralSound().ImportSound("Cammy_KO.mp3", pathFinder.GetRelativePath("Cammy_KO.mp3"));
	}

	CreateObject<SelectCRT>();
	isCharacterLoaded_ = false;
	isRoundStart_ = false;
	isLastMusicOn_ = false;
	interTimeA_ = 0.0f;
	isLastMusicOn_ = false;
	isWinSoundPlayed_ = false;
	isNameSoundPlayed_ = false;
	soundRoundOn_ = false;
	soundNumberOn_ = false;
	soundFightOn_ = false;
	CharacterLoading();
	bgmPlayer_->PlayCountReset(1);
	refereePlayer_->PlayCountReset(1);

	// 하단은 디버깅용 키입니다. 릴리즈시 제거 요망
	if (false == InputManager::GetInputManager().IsButtonKey("DebugSkip"))
	{
		InputManager::GetInputManager().CreateInputKey("DebugSkip", '6');
	}
}

void FightScene::SceneUpdateBefore()
{
	if (true == FightScene::isFightSceneNeedtobeReset_)
	{
		Reset();
		roundCount_ = 0;
		p1WinPoint_ = 0;
		p2WinPoint_ = 0;
		p1_->RestoreHP();
		p2_->RestoreHP();
		isFightSceneNeedtobeReset_ = false;
		bgmPlayer_->PlayCountReset(1);
		refereePlayer_->PlayCountReset(1);
	}
}

void FightScene::CharacterLoading()
{
	// "플레이어 1 캐릭터 선택결과" 조건문
	if (CharacterSelectScene::p1Selection_ == "")
	{
		return;
	}
	if (CharacterSelectScene::p1Selection_ == "Ryu")
	{
		Ryu* p1 = CreateObject<Ryu>("P1");
		p1->SetPos({ 280.0f, 450.0f }); // 1P 기본 위치입니다.
		p1->KeyInit();
		p1->Off();
		SetCharacterHandleP1(p1);
	}
	if (CharacterSelectScene::p1Selection_ == "Ken")
	{
		Ken* p1 = CreateObject<Ken>("P1");
		p1->SetPos({ 280.0f, 450.0f }); // 1P 기본 위치입니다.
		p1->KeyInit();
		p1->Off();
		SetCharacterHandleP1(p1);
	}
	if (CharacterSelectScene::p1Selection_ == "ChunLi")
	{
		ChunLi* p1 = CreateObject<ChunLi>("P1");
		p1->SetPos({ 280.0f, 450.0f }); // 1P 기본 위치입니다.
		p1->KeyInit();
		p1->Off();
		SetCharacterHandleP1(p1);
	}
	if (CharacterSelectScene::p1Selection_ == "Cammy")
	{
		Cammy* p1 = CreateObject<Cammy>("P1");
		p1->SetPos({ 280.0f, 450.0f }); // 1P 기본 위치입니다.
		p1->KeyInit();
		p1->Off();
		SetCharacterHandleP1(p1);
	}


	// "플레이어 2 캐릭터 선택결과" 조건문
	if (CharacterSelectScene::p2Selection_ == "Ryu")
	{
		Ryu* p2 = CreateObject<Ryu>("P2"); // 조건문에 따라 템플릿명을 캐릭터명으로 바꿔 줍니다.
		p2->SetPos({ 872.0f, 450.0f }); // 2P 기본 위치입니다.
		p2->KeyInit(); // 해당 캐릭터의 키 객체화
		p2->Off();
		//Projectile* p2Projectile = CreateObject<Projectile>("P2Projectile");
		//p2Projectile->CollisionInit();
		//p2Projectile->SetParentCharacter(p2);
		SetCharacterHandleP2(p2);
		CreateObject<StageRyu>();
		CreateObject<StageRyuMid>();
		CreateObject<StageRyuBack>();
	}
	if (CharacterSelectScene::p2Selection_ == "Ken")
	{
		Ken* p2 = CreateObject<Ken>("P2"); // 조건문에 따라 템플릿명을 캐릭터명으로 바꿔 줍니다.
		p2->SetPos({ 872.0f, 450.0f }); // 2P 기본 위치입니다.
		p2->KeyInit(); // 해당 캐릭터의 키 객체화
		p2->Off();
		SetCharacterHandleP2(p2);
		CreateObject<StageGuileBack>();
		CreateObject<StageGuileFog>();
		CreateObject<StageGuileMid>();
		CreateObject<StageGuileFore>();
		StageGuilePeople* people1 = CreateObject<StageGuilePeople>();
		StageGuilePeople* people2 = CreateObject<StageGuilePeople>();
		StageGuilePeople* people3 = CreateObject<StageGuilePeople>();
		people1->SetPos({ 576,400 });
		people2->SetPos({ 176,430 });
		people2->ChangeAnimation("People2");
		people3->SetPos({ 1006,430 });
		people3->ChangeAnimation("People3");
	}
	if (CharacterSelectScene::p2Selection_ == "ChunLi")
	{
		ChunLi* p2 = CreateObject<ChunLi>("P2"); // 조건문에 따라 템플릿명을 캐릭터명으로 바꿔 줍니다.
		p2->SetPos({ 872.0f, 450.0f }); // 2P 기본 위치입니다.
		p2->KeyInit(); // 해당 캐릭터의 키 객체화
		p2->Off();
		SetCharacterHandleP2(p2);
		CreateObject<StageChunLiBack>();
		CreateObject<StageChunLiFore>();
	}
	if (CharacterSelectScene::p2Selection_ == "Cammy")
	{
		Cammy* p2 = CreateObject<Cammy>("P2");
		p2->SetPos({ 872.0f, 450.0f }); // 2P 기본 위치입니다.
		p2->KeyInit(); // 해당 캐릭터의 키 객체화
		p2->Off();
		SetCharacterHandleP2(p2);
		CreateObject<StageCammyBack>();
		CreateObject<StageCammy>();

	}

	Shadow* p1Shadow = CreateObject<Shadow>();
	Shadow* p2Shadow = CreateObject<Shadow>();

	p1Shadow->SetParentCharacter(p1_);
	p2Shadow->SetParentCharacter(p2_);
	p1Shadow->PosInit();
	p2Shadow->PosInit();

	// 체력 바 아래 UI 구현
	CreateObject<HitPointGauge>();
	// 플레이어 체력바 구현
	HPbar* p1HP = CreateObject<HPbar>("P1HPbar");
	p1HP->SetPos({ 312,77 });
	p1HP->SetOriginalPos(312, 77);
	HPbar* p2HP = CreateObject<HPbar>("P2HPbar");
	p2HP->SetPos({ 840,77 });
	p2HP->SetOriginalPos(840, 77);

	p1HPname_ = CreateObject<HitPointName>();
	p2HPname_ = CreateObject<HitPointName>();
	p1HPname_->SetPos({150, 110});
	p2HPname_->SetPos({1002, 110});
	p1HPname_->SetHitPointName(CharacterSelectScene::p1Selection_);
	p2HPname_->SetHitPointName(CharacterSelectScene::p2Selection_);

	RefereeText* refreeText = CreateObject<RefereeText>("RefreeText");
	refreeText->SetPos({ 576, 336 });
	TimeCounter* tenDigitsCounter = CreateObject<TimeCounter>("Tendigits");
	TimeCounter* oneDigitsCounter = CreateObject<TimeCounter>("Onedigits");
	tenDigitsCounter->SetPos({ 561 ,117 });
	oneDigitsCounter->SetPos({ 591, 117 });

	fadeObject_ = CreateObject<ScreenFade>("ScreenFade");

	FightEffects* p1Effects = CreateObject<FightEffects>("P1Effects");
	FightEffects* p2Effects = CreateObject<FightEffects>("P2Effects");
	p1_->SetOtherCharacter(p2_);
	p1_->SetHPbar(p1HP);
	p1_->SetFightEffects(p1Effects);
	p1Effects->SetParentCharacter(p1_);
	p1HP->SetParentCharacter(p1_);
	SetHPbarP1(p1HP);

	p2_->SetOtherCharacter(p1_);
	p2_->SetHPbar(p2HP);
	p2_->SetFightEffects(p2Effects);
	p2Effects->SetParentCharacter(p2_);

	p2HP->SetParentCharacter(p2_);
	SetHPbarP1(p2HP);

	Winpoint* p1Winpoint = CreateObject<Winpoint>();
	p1Winpoint->SetName("P1");
	Winpoint* p2Winpoint = CreateObject<Winpoint>();
	p2Winpoint->SetName("P2");
	p1Winpoint->Off();
	p2Winpoint->Off();

	SetTimeCounter(tenDigitsCounter, oneDigitsCounter);
	SetRefereeText(refreeText);
	timeCounterTen_->Off();
	timeCounterOne_->Off();

	isCharacterLoaded_ = true;
}



void FightScene::SceneUpdate() 
{
	if (false == isCharacterLoaded_)
	{
		CharacterLoading();
	}

	if (CharacterSelectScene::p2Selection_ == "Ken")
	{
		//bgmPlayer_->Stop();
		bgmPlayer_->PlayAlone("ThemeGuile", 1);
		nowPlaying_ = "ThemeGuile";
	}
	if (CharacterSelectScene::p2Selection_ == "Ryu")
	{
		//bgmPlayer_->Stop();
		bgmPlayer_->PlayAlone("ThemeRyu", 1);
		nowPlaying_ = "ThemeRyu";
	}
	if (CharacterSelectScene::p2Selection_ == "ChunLi")
	{
		//bgmPlayer_->Stop();
		bgmPlayer_->PlayAlone("ThemeChunLi", 1);
		nowPlaying_ = "ThemeChunLi";
	}
	if (CharacterSelectScene::p2Selection_ == "Cammy")
	{
		bgmPlayer_->PlayAlone("ThemeCammy", 1);
		nowPlaying_ = "ThemeCammy";
	}

	if (true == InputManager::GetInputManager().IsButtonDown("TestKey"))
	{
	}

	if (isCharacterLoaded_ == true)
	{
		if (70 > p1_->GetCurHp() || 70 > p2_->GetCurHp())
		{
			if (1 <= p1WinPoint_ || 1 <= p2WinPoint_)
			{
				if (false == isLastMusicOn_)
				{
					bgmPlayer_->Stop();
					bgmPlayer_->PlayCountReset(1);
					bgmPlayer_->PlayAlone(nowPlaying_ + "Last", 0);
					isLastMusicOn_ = true;
				}
			}
		}



		if (0 >= p1_->GetCurHp())
		{
			p2_->isSessionOver_ = true;
			timeCounterTen_->Off();
			timeCounterOne_->Off();
			if (true == isLastMusicOn_)
			{
				bgmPlayer_->Stop();
			}
			
			if (true == p2_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("R1WinL") || true == p2_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("R1WinR")
				|| true == p2_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("MatchPointWinL") || true == p2_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("MatchPointWinR"))
			{
				interTimeA_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				if (false == isNameSoundPlayed_)
				{
					refereePlayer_->PlayCountReset(1);
					refereePlayer_->PlayOverlap("Referee" + CharacterSelectScene::p2Selection_ + ".mp3", 0);
					isNameSoundPlayed_ = true;
				}
				if (false == isWinSoundPlayed_ && 0.8f < interTimeA_)
				{
					refereePlayer_->PlayCountReset(1);
					refereePlayer_->PlayOverlap("RefereeWin.mp3", 0);
					isWinSoundPlayed_ = true;
				}
	
				refereeTextHandle_->ChangeText(CharacterSelectScene::p2Selection_ + "WinSign.bmp");
				refereeTextHandle_->SetPos({ 576, 236 });
				refereeTextHandle_->On();

				
				if (3.0f == interTimeA_)
				{
					fadeObject_->FadeIn();
				}
				if (6.0f <= interTimeA_)
				{
					p2WinPoint_++;

					if (2 == p2WinPoint_)
					{
						whoWon_ = "P2";
						winnerName_ = CharacterSelectScene::p2Selection_;
						loserName_ = CharacterSelectScene::p1Selection_;
						return SceneManager::GetSceneManager().ChangeScene("ResultScene");
					}
					else
					{
						Reset();
						isCharacterLoaded_ = false;
						isRoundStart_ = false;
						isLastMusicOn_ = false;
						interTimeA_ = 0.0f;
						isWinSoundPlayed_ = false;
						isNameSoundPlayed_ = false;
						return;
					}
				}
			}
		}

		if (0 >= p2_->GetCurHp())
		{
			p1_->isSessionOver_ = true;
			timeCounterTen_->Off();
			timeCounterOne_->Off();
			if (true == isLastMusicOn_)
			{
				bgmPlayer_->Stop();
			}

			if (true == p1_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("R1WinL") || true == p1_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("R1WinR") 
				|| true == p1_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("MatchPointWinL") || true == p1_->GetMainSpriteRenderer()->IsCurSpriteAnimationName("MatchPointWinR"))
			{
				interTimeA_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				if (false == isNameSoundPlayed_)
				{
					refereePlayer_->PlayCountReset(1);
					refereePlayer_->PlayOverlap("Referee" + CharacterSelectScene::p1Selection_ + ".mp3", 0);
					isNameSoundPlayed_ = true;
				}
				if (false == isWinSoundPlayed_ && 0.8f < interTimeA_)
				{
					refereePlayer_->PlayCountReset(1);
					refereePlayer_->PlayOverlap("RefereeWin.mp3", 0);
					isWinSoundPlayed_ = true;
				}

				refereeTextHandle_->ChangeText(CharacterSelectScene::p1Selection_ + "WinSign.bmp");
				refereeTextHandle_->SetPos({ 576, 236 });
				refereeTextHandle_->On();

				if (3.0f == interTimeA_)
				{
					fadeObject_->FadeIn();
				}
				if (6.0f <= interTimeA_)
				{
					p1WinPoint_++;
					if (2 == p1WinPoint_)
					{
						whoWon_ = "P1";
						winnerName_ = CharacterSelectScene::p1Selection_;
						loserName_ = CharacterSelectScene::p2Selection_;
						return SceneManager::GetSceneManager().ChangeScene("ResultScene");
					}
					else
					{
						Reset();
						isCharacterLoaded_ = false;
						isRoundStart_ = false;
						interTimeA_ = 0.0f;
						isLastMusicOn_ = false;
						isWinSoundPlayed_ = false;
						isNameSoundPlayed_ = false;
						return;
					}
				}
			}
		}

		if (0 == roundCount_) // 라운드카운트가 0일때부터 시작
		{
			if (false == soundRoundOn_)
			{
				refereePlayer_->PlayOverlap("RefereeRound.mp3", 0);
				soundRoundOn_ = true;
			}
			if (0.5f < roundInterTime_ && false == soundNumberOn_)
			{
				refereePlayer_->PlayCountReset(1);
				refereePlayer_->PlayOverlap("RefereeOne.mp3", 0);
				soundNumberOn_ = true;
			}
			if (1.5f <= roundInterTime_)
			{
				if (false == soundFightOn_)
				{
					refereePlayer_->PlayCountReset(1);
					refereePlayer_->PlayOverlap("RefereeFight.mp3", 0);
					soundFightOn_ = true;
				}
				refereeTextHandle_->ChangeText("Fight.bmp");
				roundInterTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				if (3.0f <= roundInterTime_)
				{
					refereeTextHandle_->Off();
					p1_->On();
					p2_->On();
					roundCount_ = 1;
					isRoundStart_ = true;
					roundInterTime_ = 0.0f;
					timeCounterTen_->On();
					timeCounterOne_->On();
					return;
				}

				return;
			}
			roundInterTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			refereeTextHandle_->On();
			refereeTextHandle_->ChangeText("R1.bmp");
			return;
		}

		if (1 == roundCount_ && false == isRoundStart_) // 라운드 2
		{
			if (false == soundRoundOn_)
			{
				refereePlayer_->PlayOverlap("RefereeRound.mp3", 0);
				soundRoundOn_ = true;
			}
			if (0.5f < roundInterTime_ && false == soundNumberOn_)
			{
				refereePlayer_->PlayCountReset(1);
				refereePlayer_->PlayOverlap("RefereeTwo.mp3", 0);
				soundNumberOn_ = true;
			}
			if (1.5f <= roundInterTime_)
			{
				if (false == soundFightOn_)
				{
					refereePlayer_->PlayCountReset(1);
					refereePlayer_->PlayOverlap("RefereeFight.mp3", 0);
					soundFightOn_ = true;
				}
				refereeTextHandle_->ChangeText("Fight.bmp");
				roundInterTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				if (3.0f <= roundInterTime_)
				{
					refereeTextHandle_->Off();
					p1_->On();
					p2_->On();
					roundCount_ = 2;
					isRoundStart_ = true;
					roundInterTime_ = 0.0f;
					timeCounterTen_->On();
					timeCounterOne_->On();

					return;
				}

				return;
			}
			roundInterTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			refereeTextHandle_->On();
			refereeTextHandle_->ChangeText("R2.bmp");
			return;
		}

		if (2 == roundCount_ && false == isRoundStart_ && 2 != p1WinPoint_ && 2 != p2WinPoint_) // 라운드 3
		{
			if (false == soundRoundOn_)
			{
				refereePlayer_->PlayOverlap("RefereeFinal.mp3", 0);
				soundRoundOn_ = true;
			}
			if (0.5f < roundInterTime_ && false == soundNumberOn_)
			{
				refereePlayer_->PlayCountReset(1);
				refereePlayer_->PlayOverlap("RefereeRound.mp3", 0);
				soundNumberOn_ = true;
			}
			if (1.5f <= roundInterTime_)
			{
				if (false == soundFightOn_)
				{
					refereePlayer_->PlayCountReset(1);
					refereePlayer_->PlayOverlap("RefereeFight.mp3", 0);
					soundFightOn_ = true;
				}
				refereeTextHandle_->ChangeText("Fight.bmp");
				roundInterTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
				if (3.0f <= roundInterTime_)
				{
					refereeTextHandle_->Off();
					p1_->On();
					p2_->On();
					roundCount_ = 3;
					isRoundStart_ = true;
					roundInterTime_ = 0.0f;
					timeCounterTen_->On();
					timeCounterOne_->On();
					return;
				}

				return;
			}
			roundInterTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			refereeTextHandle_->On();
			refereeTextHandle_->ChangeText("R3.bmp");
			return;
		}

		// *** 하단부터 캐릭터 이동 시 카메라 이동 기능입니다.
		if (-1 == p1_->GetCurDirectionX() && 152.0f <= p1_->GetPos().x)
			// CurDirectionX 가 1이면 오른쪽, -1면 왼쪽으로 플레이어가 이동중입니다.
		{
			if (280.0f >= p1_->GetCameraLinkedPos().x)
			{
				if (1067.0f >= p2_->GetCameraLinkedPos().x)
				
				{
					MoveCameraPos(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p1_->GetMoveSpeed());
				}
			}
			else if (1067.0f <= p1_->GetCameraLinkedPos().x && 280.0f >= p1_->GetCameraLinkedPos().x && 152.0f <= p1_->GetPos().x)
			{
				MoveCameraPos(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p1_->GetMoveSpeed());
			}

		}
		if (-1 == p2_->GetCurDirectionX() && 152.0f <= p2_->GetPos().x)
		{
			if (280.0f >= p2_->GetCameraLinkedPos().x)
			{
				if (1067.0f >= p1_->GetCameraLinkedPos().x)
				{
					MoveCameraPos(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p2_->GetMoveSpeed());
				}
			}
			else if (1067.0f <= p2_->GetCameraLinkedPos().x && 280.0f >= p1_->GetCameraLinkedPos().x && 152.0f <= p1_->GetPos().x)
			{
				MoveCameraPos(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p2_->GetMoveSpeed());
			}

		}
		if (1 == p2_->GetCurDirectionX() && 1000.0f >= p2_->GetPos().x)
		{
			if (872.0f <= p2_->GetCameraLinkedPos().x)
			{
				if (85.0f <= p1_->GetCameraLinkedPos().x)
				{
					MoveCameraPos(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p2_->GetMoveSpeed());
				}
			}
			else if (85.0f >= p2_->GetCameraLinkedPos().x && 872.0f <= p1_->GetCameraLinkedPos().x && 1000.0f >= p1_->GetPos().x)
			{
				MoveCameraPos(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p2_->GetMoveSpeed());
			}

		}
		if (1 == p1_->GetCurDirectionX() && 1000.0f >= p1_->GetPos().x)
		{
			if (872.0f <= p1_->GetCameraLinkedPos().x)
			{
				if (85.0f <= p2_->GetCameraLinkedPos().x)
				{
					MoveCameraPos(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p1_->GetMoveSpeed());
				}
			}
			else if (85.0f >= p1_->GetCameraLinkedPos().x && 872.0f <= p2_->GetCameraLinkedPos().x && 1000.0f >= p2_->GetPos().x)
			{
				MoveCameraPos(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p1_->GetMoveSpeed());

			}
		}

		if (1 == p1_->GetCurDirectionY() && 240.0f >= p1_->GetPos().y)
		{ // 역시 플레이어의 CurDirectionY 가 1이면 상승 중, -1이면 하강 중입니다.
			//float a = GetCameraPos().y;
			MoveCameraPos(float4::UP * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p1_->GetJumpForce() * 0.1f);
			if (0 <= GetCameraPos().y)
			{
				SetCameraPos({ GetCameraPos().x, 0 });
			}
		}

		if (-1 == p1_->GetCurDirectionY() && 240.0f <= p1_->GetPos().y)
		{
			MoveCameraPos(float4::DOWN * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * p1_->GetJumpForce() * 0.1f);
			if (0 <= GetCameraPos().y)
			{
				SetCameraPos({ GetCameraPos().x, 0 });
			}

		}
		if (1 == p2_->GetCurDirectionY() && 240.0f >= p2_->GetPos().y && 1 != p1_->GetCurDirectionY())
		{
			MoveCameraPos(float4::UP * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f);
			if (0 <= GetCameraPos().y)
			{
				SetCameraPos({ GetCameraPos().x, 0 });
			}
		}
		if (-1 == p2_->GetCurDirectionY() && 240.0f <= p2_->GetPos().y && -1 != p1_->GetCurDirectionY())
		{
			MoveCameraPos(float4::DOWN * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f);
			if (0 <= GetCameraPos().y)
			{
				SetCameraPos({ GetCameraPos().x, 0 });
			}
		}
		// 카메라 이동 구현 끝 ***
	}
	
	// 디버깅용 조건문입니다.
	if (true == InputManager::GetInputManager().IsButtonDown("DebugSkip"))
	{
		if (1 == roundCount_)
		{
			p1WinPoint_++;
			Reset();
			isCharacterLoaded_ = false;
			isRoundStart_ = false;
			isLastMusicOn_ = false;
			interTimeA_ = 0.0f;
			isWinSoundPlayed_ = false;
			return;
		}
		if (2 == roundCount_)
		{
			p2WinPoint_++;
			Reset();
			isCharacterLoaded_ = false;
			isRoundStart_ = false;
			isLastMusicOn_ = false;
			interTimeA_ = 0.0f;
			isWinSoundPlayed_ = false;
			return;

		}
		if (3 == roundCount_)
		{
			bgmPlayer_->Stop();
			p1WinPoint_++;
			whoWon_ = "P1";
			winnerName_ = CharacterSelectScene::p1Selection_;
			loserName_ = CharacterSelectScene::p2Selection_;
			SceneManager::GetSceneManager().ChangeScene("ResultScene");
		}
	}
}

void FightScene::SceneRenderAfter()
{
	// Sleep(1);

	ImageAsset* backBufferImage = AssetManager::GetAssetManager().GetNextBuffer();

	for (int i = 0; i < DebugPos.size(); i++)
	{
		Rectangle(backBufferImage->GetDC(),
			DebugPos[i].ix() - 5,
			DebugPos[i].iy() - 5,
			DebugPos[i].ix() + 5,
			DebugPos[i].iy() + 5);
	}
}