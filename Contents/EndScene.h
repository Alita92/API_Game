#pragma once
#include <Scene.h>
// �з� :
// �뵵 :
// ���� :
class ScreenFade;
class EngineGeneralSoundPlayer;
class EndScene : public Scene
{
private:	// member Var
	EngineGeneralSoundPlayer* bgmPlayer_;
	EngineGeneralSoundPlayer* soundPlayer_;
	float interTime_;
	static ScreenFade* fadeObject_;

public:		
	EndScene(); // default constructer ����Ʈ ������
	~EndScene(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EndScene(const EndScene& _other) = delete; // default Copy constructer ����Ʈ ���������
	EndScene(EndScene&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EndScene& operator=(const EndScene& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EndScene& operator=(const EndScene&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Loading() override;
	void SceneUpdate() override;
};

