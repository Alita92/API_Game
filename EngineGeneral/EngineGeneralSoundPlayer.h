#pragma once
#include "EngineGeneralSoundFile.h"
#include "EngineGeneralSound.h"

// 분류 :
// 용도 :
// 설명 :
class EngineGeneralSoundPlayer
{
	friend EngineGeneralSound;

private:	// member Var
	EngineGeneralSound* playSoundFile_;
	FMOD::Channel* playChannel_;
	int playCount_;

public:		
	EngineGeneralSoundPlayer(); // default constructer 디폴트 생성자
	~EngineGeneralSoundPlayer(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralSoundPlayer(const EngineGeneralSoundPlayer& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EngineGeneralSoundPlayer(EngineGeneralSoundPlayer&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralSoundPlayer& operator=(const EngineGeneralSoundPlayer& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralSoundPlayer& operator=(const EngineGeneralSoundPlayer&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	bool IsPlay();
	void PlayCountReset(int _count = -1);
	void PlayOverlap(const std::string& _name, int _LoopCount = 1);
	void PlayAlone(const std::string& _name, int _LoopCount = 1);
	void Stop();
};

