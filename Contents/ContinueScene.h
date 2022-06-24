#pragma once
#include <Scene.h>
// �з� : ��üȭ�� Scene ����, 1P�� 2P �� �й踦 ���Ϲ��� �÷��̾��� �÷��� ���� ���θ� �����ϴ�.
// �뵵 : ���� �ð�(10��) �� �־�����, �ǽð����� ī��Ʈ ���� ������Ʈ�� ������Ʈ�Ǿ� ǥ�õ˴ϴ�.
//		  1P�� 2P �� �й踦 ���Ϲ��� �÷��̾ ������ ������ ĳ������ ��ģ ��Ʈ����Ʈ�� ǥ�õ˴ϴ�.
//		  ���� �ð� �ʰ� �� <��ŸƮ> �Է��� ������ ��Ʈ����Ʈ�� �� ������ ���� �� ��� ��Ʈ����Ʈ�� ǥ�õ˴ϴ�.
// 
// ���� : ���� �ð� �ʰ� �� <��ŸƮ> �Է��� ������ "CharacterSelectScene" ���� ���� ��ȯ�մϴ�.
//		  ���� �ð� �ʰ� ������ <��ŸƮ> �Է��� ������ "TitleScene" ���� ���� ��ȯ�մϴ�.
class ScreenFade;
class ContinueKOCounter;
class ContinueLoserPortrait;
class EngineGeneralSoundPlayer;
class ContinueScene : public Scene
{
private:	// member Var
	EngineGeneralSoundPlayer* bgmPlayer_;
	EngineGeneralSoundPlayer* soundPlayer_;
	float interTime_;
	float interTimeToChangeScene_;
	int timeSwitchNum_;
	bool isBgmPlaying_;

	bool isGameOver_;
	bool isSceneChange_;
	ContinueKOCounter* continueKOCounter_;
	ContinueLoserPortrait* continueLoserPortrait_;
	static ScreenFade* fadeObject_;

public:		
	ContinueScene(); // default constructer ����Ʈ ������
	~ContinueScene(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ContinueScene(const ContinueScene& _other) = delete; // default Copy constructer ����Ʈ ���������
	ContinueScene(ContinueScene&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ContinueScene& operator=(const ContinueScene& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ContinueScene& operator=(const ContinueScene&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Loading() override;
	void SceneUpdateBefore() override;
	void SceneUpdate() override;
};

