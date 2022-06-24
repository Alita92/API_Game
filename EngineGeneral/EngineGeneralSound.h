#pragma once
#include <FMOD/fmod.hpp>
#include <list>
#include <map>
#include <string>

#pragma comment(lib, "FMOD/fmod_vc.lib") // Release용 빌드된 코드파일

// 분류 :
// 용도 :
// 설명 :
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
	FMOD::System* soundSystem_;  // Fmod가 제공해주는 인터페이스
	// 사운드파일하나하나가 아니라
	// fmod를 사용할수 있는지 확인해주고 사운드를 로드할수 있게 해주는 기본 인터페이스입니다.
	// 이녀석이 먼저 제대로 만들어질수 있는 환경이어야 사운드를 사용할수 있습니다.
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
	EngineGeneralSound(); // default constructer 디폴트 생성자
	~EngineGeneralSound(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralSound(const EngineGeneralSound& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EngineGeneralSound(EngineGeneralSound&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralSound& operator=(const EngineGeneralSound& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralSound& operator=(const EngineGeneralSound&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
};

