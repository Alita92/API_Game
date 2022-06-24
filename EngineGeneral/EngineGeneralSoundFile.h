#pragma once
#include "EngineGeneralSound.h"
#include "EngineGeneralNameBase.h"

// �з� :
// �뵵 :
// ���� :
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
	EngineGeneralSoundFile(); // default constructer ����Ʈ ������
	~EngineGeneralSoundFile(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralSoundFile(const EngineGeneralSoundFile& _other) = delete; // default Copy constructer ����Ʈ ���������
	EngineGeneralSoundFile(EngineGeneralSoundFile&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralSoundFile& operator=(const EngineGeneralSoundFile& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralSoundFile& operator=(const EngineGeneralSoundFile&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

