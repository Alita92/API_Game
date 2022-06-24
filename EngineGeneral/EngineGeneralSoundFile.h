#pragma once
#include "EngineGeneralSound.h"
#include "EngineGeneralNameBase.h"

// 분류 :
// 용도 :
// 설명 :
class EngineGeneralSoundPlayer;
class EngineGeneralSoundFile : public EngineGeneralNameBase
{
public:
	friend EngineGeneralSound;
	friend EngineGeneralSoundPlayer;

private:	// member Var
	FMOD::Sound* sound_;

public:
	bool Load(const std::string& _path);


public:		
	EngineGeneralSoundFile(); // default constructer 디폴트 생성자
	~EngineGeneralSoundFile(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralSoundFile(const EngineGeneralSoundFile& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EngineGeneralSoundFile(EngineGeneralSoundFile&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralSoundFile& operator=(const EngineGeneralSoundFile& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralSoundFile& operator=(const EngineGeneralSoundFile&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
};

