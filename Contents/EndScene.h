#pragma once
#include <Scene.h>
// 분류 :
// 용도 :
// 설명 :
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
	EndScene(); // default constructer 디폴트 생성자
	~EndScene(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EndScene(const EndScene& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EndScene(EndScene&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EndScene& operator=(const EndScene& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EndScene& operator=(const EndScene&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void Loading() override;
	void SceneUpdate() override;
};

