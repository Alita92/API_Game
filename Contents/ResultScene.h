#pragma once
#include <Scene.h>
// �з� : ��üȭ�� Scene ����, 1P�� 2P�� ���� ������ ���մϴ�.
// �뵵 : ������ ������ ���� �¸��� �÷��̾��� ĳ������ �Ϲ� ��Ʈ����Ʈ, �й��� �÷��̾��� ĳ������ ��ģ ��Ʈ����Ʈ�� ǥ���մϴ�.
//        �¸��� �÷��̾��� ĳ���Ͱ� ���� <�¸� ���> �� ����մϴ�.
// ���� : ������ �ð�(����) ���� 1P�� 2P�� ���õ� ĳ������ ��Ʈ����Ʈ�� ���·� ���и� Ȯ���� �� �ֽ��ϴ�. �ð� �ʰ��� "ContinueScene" ���� ���� ��ȯ�մϴ�.
//        �̵� Ű�� ������ ��� �Է��� ���� �� ���� ����� ��ŵ �� �ٷ� "ContinueScene" ���� ���� ��ȯ�մϴ�.
class ScreenFade;
class EngineGeneralSoundPlayer;
class ResultBackground;
class ResultScene : public Scene
{
private:	// member Var
	static ScreenFade* fadeObject_;
	float interTime_;
	EngineGeneralSoundPlayer* bgmPlayer_;

	bool isSceneChange_;

	ResultBackground* resultBackground1_;
	ResultBackground* resultBackground2_;
	ResultBackground* resultBackground3_;

public:
	static bool isResultSceneNeedtobeReset_;

public:		
	ResultScene(); // default constructer ����Ʈ ������
	~ResultScene(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ResultScene(const ResultScene& _other) = delete; // default Copy constructer ����Ʈ ���������
	ResultScene(ResultScene&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ResultScene& operator=(const ResultScene& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ResultScene& operator=(const ResultScene&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Loading() override;
	void SceneUpdateBefore() override;
	void SceneUpdate() override;
};

