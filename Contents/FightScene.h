#pragma once
#include <Scene.h>
#include <vector>

// �з� : ��üȭ�� Scene ����, ���� �÷����� �� �ִ� ȭ���Դϴ�.
// �뵵 : ü�� �������� ���� �޺� ������, ���� �ð��� ���� ���� UI ������Ʈ�� ǥ�õ˴ϴ�.
//		  2P ĳ������ ������ ��� ��������Ʈ�� ��������� ����˴ϴ�.
//		  1P �� 2P�� ���õ� ĳ������ ��������Ʈ�� ǥ�õǸ�, �̸� �����Ͽ� ������ ü���� 0���� ����� ������ �����մϴ�.
//		  �� ���� ������ 2�� �ȴٸ� ���и� �����մϴ�.
//        ���� �ð� 60�ʰ� �־�����, �ð� �ʰ� ������ ������ �÷��̾��� ü���� 0�� ���� ������, ü�°��� ���� ������ �����մϴ�.
// ���� : 1P �� 2P�� <��,��,����ġ> <��,��,��ű> <����Ű> <����Ű1,2> �� ����� �� ������, ���� ������ 2�� ���� �� �¸��մϴ�.
//        �¸� �� "ResultScene" ���� ���� ��ȯ�մϴ�.
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
	FightScene(); // default constructer ����Ʈ ������
	~FightScene(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	FightScene(const FightScene& _other) = delete; // default Copy constructer ����Ʈ ���������
	FightScene(FightScene&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	FightScene& operator=(const FightScene& _other) = delete; // default Copy operator ����Ʈ ���� ������
	FightScene& operator=(const FightScene&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

