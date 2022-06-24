#pragma once
#include "EngineGeneralSoundFile.h"
#include "EngineGeneralSound.h"

// �з� :
// �뵵 :
// ���� :
class EngineGeneralSoundPlayer
{
	friend EngineGeneralSound;

private:	// member Var
	EngineGeneralSound* playSoundFile_;
	FMOD::Channel* playChannel_;
	int playCount_;

public:		
	EngineGeneralSoundPlayer(); // default constructer ����Ʈ ������
	~EngineGeneralSoundPlayer(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralSoundPlayer(const EngineGeneralSoundPlayer& _other) = delete; // default Copy constructer ����Ʈ ���������
	EngineGeneralSoundPlayer(EngineGeneralSoundPlayer&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralSoundPlayer& operator=(const EngineGeneralSoundPlayer& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralSoundPlayer& operator=(const EngineGeneralSoundPlayer&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	bool IsPlay();
	void PlayCountReset(int _count = -1);
	void PlayOverlap(const std::string& _name, int _LoopCount = 1);
	void PlayAlone(const std::string& _name, int _LoopCount = 1);
	void Stop();
};

