#pragma once
#include <Scene.h>

// �з� : ��üȭ�� Scene ����, Ÿ��Ʋ ȭ���Դϴ�.
// �뵵 : ��Ʈ�� �������� ����Ǹ�, ������ ����� ������ Ÿ��Ʋ �ΰ� ǥ�õǸ� �ϴܿ� "Insert Coin" ������Ʈ�� �����մϴ�.
//		  Ÿ��Ʋ �ΰ� ǥ�� �� �ƹ� �Է��� ������ 15�� �� ��Ʈ�� �������� �ٽ� ����˴ϴ�.
// ���� : ��Ʈ�� ������ ����� ������ �� <���� ����> ��ư�� �Է� ��, �������� ��ŵ�ǰ� �ٷ� Ÿ��Ʋ �ΰ� ȭ���� ǥ�õ˴ϴ�.
//		  <���� ����> ��ư�� 1ȸ �̻� �Էµ� �Ŀ���, <��ŸƮ> ��ư�� �Է��Ͽ� ������Ʈ���� ���̵� �ƿ� ó���ϰ� "CharacterSelectScene" ���� ���� ��ȯ�˴ϴ�.

class TitleTypoSprites;
class ScreenFade;
class EngineGeneralSoundPlayer;
class TitleScene : public Scene
{
private:	// member Var
	static ScreenFade* fadeObject_;
	EngineGeneralSoundPlayer* soundPlayer_;

	float interTime_;
	bool isCoin_;
	bool isStart_;

	TitleTypoSprites* typo_;
public:		
	TitleScene(); // default constructer ����Ʈ ������
	~TitleScene(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	TitleScene(const TitleScene& _other) = delete; // default Copy constructer ����Ʈ ���������
	TitleScene(TitleScene&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	TitleScene& operator=(const TitleScene& _other) = delete; // default Copy operator ����Ʈ ���� ������
	TitleScene& operator=(const TitleScene&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Loading() override;
	void SceneUpdate() override;
};

