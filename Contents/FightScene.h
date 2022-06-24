#pragma once
#include <Scene.h>
#include <vector>

// 분류 : 구체화된 Scene 으로, 실제 플레이할 수 있는 화면입니다.
// 용도 : 체력 게이지와 슈퍼 콤보 게이지, 제한 시간과 점수 등의 UI 오브젝트가 표시됩니다.
//		  2P 캐릭터의 지정된 배경 스프라이트와 배경음악이 재생됩니다.
//		  1P 와 2P의 선택된 캐릭터의 스프라이트가 표시되며, 이를 조작하여 상대방의 체력을 0으로 만들면 승점을 리턴합니다.
//		  한 쪽의 승점이 2가 된다면 승패를 리턴합니다.
//        제한 시간 60초가 주어지며, 시간 초과 전까지 한쪽의 플레이어의 체력이 0이 되지 않으면, 체력값을 비교해 승점을 리턴합니다.
// 설명 : 1P 와 2P는 <약,중,강펀치> <약,중,강킥> <방향키> <이지키1,2> 를 사용할 수 있으며, 먼저 승점을 2점 얻을 시 승리합니다.
//        승리 시 "ResultScene" 으로 씬을 전환합니다.
class ScreenFade;
class EngineGeneralSoundPlayer;
class CharacterHandle;
class HPbar;
class HitPointName;
class RefereeText;
class Projectile;
class Kikoken;
class TimeCounter;
class CharacterSelectScene;
class FightScene : public Scene
{
	friend CharacterHandle;

private:	// member Var
	bool isCharacterLoaded_;

	CharacterHandle* p1_;
	CharacterHandle* p2_;
	HPbar* p1HPbar_;
	HPbar* p2HPbar_;
	HitPointName* p1HPname_;
	HitPointName* p2HPname_;

	TimeCounter* timeCounterTen_;
	TimeCounter* timeCounterOne_;
	RefereeText* refereeTextHandle_;
	static ScreenFade* fadeObject_;

	EngineGeneralSoundPlayer* bgmPlayer_;
	std::string nowPlaying_;
	EngineGeneralSoundPlayer* refereePlayer_;
	bool isLastMusicOn_;

	float interTimeA_;
	float interTimeB_;

	int roundCount_;
	float roundInterTime_;
	bool isRoundStart_;
	bool isWinSoundPlayed_;
	bool isNameSoundPlayed_;


	CharacterSelectScene* characterSelectScene_;

public:
	static std::vector<float4> DebugPos;
	static std::string whoWon_;
	static std::string loserName_;
	static std::string winnerName_;
	static int p1WinPoint_;
	static int p2WinPoint_;

	static bool isFightSceneNeedtobeReset_;

private:
	bool soundRoundOn_;
	bool soundNumberOn_;
	bool soundFightOn_;

public:		
	FightScene(); // default constructer 디폴트 생성자
	~FightScene(); // default destructer 디폴트 소멸자

public:		// delete constructer
	FightScene(const FightScene& _other) = delete; // default Copy constructer 디폴트 복사생성자
	FightScene(FightScene&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	FightScene& operator=(const FightScene& _other) = delete; // default Copy operator 디폴트 대입 연산자
	FightScene& operator=(const FightScene&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetCharacterHandleP1(CharacterHandle* _handle)
	{
		p1_ = _handle;
	}
	void SetCharacterHandleP2(CharacterHandle* _handle)
	{
		p2_ = _handle;
	}
	void SetHPbarP1(HPbar* _p1)
	{
		p1HPbar_ = _p1;
	}
	void SetHPbarP2(HPbar* _p2)
	{
		p2HPbar_ = _p2;
	}
	void SetTimeCounter(TimeCounter* _timeCounterTen, TimeCounter* _timeCounterOne)
	{
		timeCounterTen_ = _timeCounterTen;
		timeCounterOne_ = _timeCounterOne;
	}
	void SetRefereeText(RefereeText* _text)
	{
		refereeTextHandle_ = _text;
	}

public:		//member Func
	void Loading() override;
	void SceneUpdateBefore() override;
	void SceneUpdate() override;
	void CharacterLoading();
	void SceneRenderAfter() override;
};

