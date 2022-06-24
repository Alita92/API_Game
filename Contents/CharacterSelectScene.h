#pragma once
#include <Scene.h>

// �з� : ��üȭ�� Scene ����, ĳ���� ���� ȭ���Դϴ�.
// �뵵 : ������ ���� �ð�(30��) �� �־�����, ���� ������Ʈ�� ĳ���� ��Ʈ���� ������Ʈ, Ŀ�� ���� ǥ�õ˴ϴ�. ĳ���� ���� ������ ����˴ϴ�.
// ���� : ������ Ŀ���� �̵����� 1P �� 2P �� ����� ĳ���͸� �����մϴ�. 
//		  ������ ���� �ð�(30��) ���� ĳ���͸� �������� �ʴ´ٸ� �ð� �ʰ� �� �� Ŀ���� ��ġ�� ������ ĳ���͸� �ڵ����� �����մϴ�.
//		  ������ �Ϸ�Ǿ��ٸ� ������Ʈ���� ���̵� �ƿ� ó���Ǹ� "VSScene" ���� ���� ��ȯ�˴ϴ�.\

class EngineGeneralSoundPlayer;
class VSTypo;
class SelectVSBackground;
class ScreenFade;
class Cursor;
class VSPortrait;
class VSName;
class CharacterSelectScene : public Scene
{
private:	// member Var
	Cursor* p1_;
	Cursor* p2_;

	std::string p1SelectedCharacter_;
	std::string p2SelectedCharacter_;

	static ScreenFade* fadeObject_;

	float interTime_;

	SelectVSBackground* vsBackground1_;
	SelectVSBackground* vsBackground2_;
	SelectVSBackground* vsBackground3_;
	VSTypo* vsTypo_;
	bool isFadein_;
	bool isVSin_;
	bool isSceneChange_;

	VSPortrait* p1VSPortrait_;
	VSPortrait* p2VSPortrait_;
	VSName* p1VSName_;
	VSName* p2VSName_;

	EngineGeneralSoundPlayer* bgmPlayer_;
	EngineGeneralSoundPlayer* soundPlayer_;
public:
	static std::string p1Selection_;
	static std::string p2Selection_;

public:
	static bool isSelectSceneNeedtobeReset_;

public:		
	CharacterSelectScene(); // default constructer ����Ʈ ������
	~CharacterSelectScene(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	CharacterSelectScene(const CharacterSelectScene& _other) = delete; // default Copy constructer ����Ʈ ���������
	CharacterSelectScene(CharacterSelectScene&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	CharacterSelectScene& operator=(const CharacterSelectScene& _other) = delete; // default Copy operator ����Ʈ ���� ������
	CharacterSelectScene& operator=(const CharacterSelectScene&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Loading() override;
	void SceneUpdateBefore() override;
	void SceneUpdate() override;

	void SetP1Cursor(Cursor* _child)
	{
		p1_ = _child;
	}

	void SetP2Cursor(Cursor* _child)
	{
		p2_ = _child;
	}

	std::string GetP1Character()
	{
		return p1SelectedCharacter_;
	}

	std::string GetP2Character()
	{
		return p2SelectedCharacter_;
	}
};

