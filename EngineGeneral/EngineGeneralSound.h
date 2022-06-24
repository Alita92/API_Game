#pragma once
#include <FMOD/fmod.hpp>
#include <list>
#include <map>
#include <string>

#pragma comment(lib, "FMOD/fmod_vc.lib") // Release�� ����� �ڵ�����

// �з� :
// �뵵 :
// ���� :
class EngineGeneralSoundPlayer;
class EngineGeneralSoundFile;
class EngineGeneralSound
{
private:	// member Var
	static EngineGeneralSound* engineGeneralSound_;

public:
	static EngineGeneralSound& GetEngineGeneralSound()
	{
		return *engineGeneralSound_;
	}

	static void DestroyMemory()
	{
		if (nullptr != engineGeneralSound_)
		{
			delete engineGeneralSound_;
			engineGeneralSound_ = nullptr;
		}
	}

public:
	friend EngineGeneralSoundFile;
	friend EngineGeneralSoundPlayer;

private:
	FMOD::System* soundSystem_;  // Fmod�� �������ִ� �������̽�
	// ���������ϳ��ϳ��� �ƴ϶�
	// fmod�� ����Ҽ� �ִ��� Ȯ�����ְ� ���带 �ε��Ҽ� �ְ� ���ִ� �⺻ �������̽��Դϴ�.
	// �̳༮�� ���� ����� ��������� �ִ� ȯ���̾�� ���带 ����Ҽ� �ֽ��ϴ�.
	std::map<std::string, EngineGeneralSoundFile*> mapSoundFile_;
	std::list<EngineGeneralSoundPlayer*> listSoundPlayer_;

private:
	EngineGeneralSoundFile* FindSound(const std::string& _name);

public:
	void SoundInitialize();
	void ImportSound(const std::string& _name, const std::string& _path);
	void PlaySoundOneTime(const std::string& _name);
	EngineGeneralSoundPlayer* CreateSoundPlayer();

public:
	void SoundUpdate();

public:		
	EngineGeneralSound(); // default constructer ����Ʈ ������
	~EngineGeneralSound(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralSound(const EngineGeneralSound& _other) = delete; // default Copy constructer ����Ʈ ���������
	EngineGeneralSound(EngineGeneralSound&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralSound& operator=(const EngineGeneralSound& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralSound& operator=(const EngineGeneralSound&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

