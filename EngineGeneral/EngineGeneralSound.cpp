#include "EngineGeneralSound.h"
#include "EngineGeneralDebug.h"
#include "EngineGeneralSoundFile.h"
#include "EngineGeneralSoundPlayer.h"
EngineGeneralSound* EngineGeneralSound::engineGeneralSound_ = new EngineGeneralSound();

// Static Var
// Static Func

// constructer destructer
EngineGeneralSound::EngineGeneralSound()
{
}

EngineGeneralSound::~EngineGeneralSound()
{
	{
		std::list<EngineGeneralSoundPlayer*>::iterator StartIter = listSoundPlayer_.begin();
		std::list<EngineGeneralSoundPlayer*>::iterator EndIter = listSoundPlayer_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != *StartIter)
			{
				delete* StartIter;
			}
		}
		listSoundPlayer_.clear();
	}

	{
		std::map<std::string, EngineGeneralSoundFile*>::iterator StartIter = mapSoundFile_.begin();
		std::map<std::string, EngineGeneralSoundFile*>::iterator EndIter = mapSoundFile_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != StartIter->second)
			{
				delete StartIter->second;
				StartIter->second = nullptr;
			}
		}
		mapSoundFile_.clear();
	}


	if (nullptr != soundSystem_)
	{
		soundSystem_->release();
		soundSystem_ = nullptr;
	}
}

EngineGeneralSound::EngineGeneralSound(EngineGeneralSound&& _other) noexcept
{
}

//member Func
EngineGeneralSoundFile* EngineGeneralSound::FindSound(const std::string& _name)
{
	std::map<std::string, EngineGeneralSoundFile*>::iterator FindIter = mapSoundFile_.find(_name);

	if (FindIter == mapSoundFile_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void EngineGeneralSound::SoundUpdate()
{
	if (nullptr == soundSystem_)
	{
		EngineGeneralDebug::MsgBoxError("SoundSystem Is null");
		return;
	}

	soundSystem_->update();
}

EngineGeneralSoundPlayer* EngineGeneralSound::CreateSoundPlayer()
{
	EngineGeneralSoundPlayer* NewSoundplayer = new EngineGeneralSoundPlayer();

	listSoundPlayer_.push_back(NewSoundplayer);

	return  NewSoundplayer;
}

void EngineGeneralSound::ImportSound(const std::string& _name, const std::string& _path)
{
	if (nullptr != FindSound(_name))
	{
		EngineGeneralDebug::MsgBoxError("Sound Load overlap error");
		return;
	}

	EngineGeneralSoundFile* newLoadSound = new EngineGeneralSoundFile();

	if (false == newLoadSound->Load(_path))
	{
		EngineGeneralDebug::MsgBoxError("Sound Load Error");
		delete newLoadSound;
		return;
	}

	mapSoundFile_.insert(
		std::map<std::string, EngineGeneralSoundFile*>::value_type(_name, newLoadSound));
}

void EngineGeneralSound::PlaySoundOneTime(const std::string& _name)
{
	EngineGeneralSoundFile* SoundPtr = FindSound(_name);

	if (nullptr == SoundPtr)
	{
		EngineGeneralDebug::MsgBoxError("PlaySound Error");
		return;
	}

	soundSystem_->playSound(SoundPtr->sound_, nullptr, false, nullptr);
}

//member Func

void EngineGeneralSound::SoundInitialize()
{
	// 내부코드에서 NEW를 할 가능성이 매우 다분해.
	FMOD::System_Create(&soundSystem_);

	if (nullptr == soundSystem_)
	{
		EngineGeneralDebug::MsgBoxError("sound system create Error");
		return;
	}

	// 동시에 32개 개수인지 사운드 채널의 의미인지를 잘 모르고 있습니다.
	// 32채널을 재생할수 있다는 의미인데 선생님도 잘 모릅니다.
	if (FMOD_OK != soundSystem_->init(32, FMOD_DEFAULT, nullptr))
	{
		EngineGeneralDebug::MsgBoxError("sound system init Error");
		return;
	}
}