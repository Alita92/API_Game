#include "EngineGeneralSoundFile.h"
#include "EngineGeneralDebug.h"

// Static Var
// Static Func

// constructer destructer
EngineGeneralSoundFile::EngineGeneralSoundFile()
	: sound_(nullptr)
{
}

EngineGeneralSoundFile::~EngineGeneralSoundFile()
{
}

EngineGeneralSoundFile::EngineGeneralSoundFile(EngineGeneralSoundFile&& _other) noexcept
	: sound_(_other.sound_)
{
}

//member Func
bool EngineGeneralSoundFile::Load(const std::string& _path)
{
	if (FMOD_RESULT::FMOD_OK !=
		EngineGeneralSound::GetEngineGeneralSound().soundSystem_->createSound(
			_path.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound_))
	{
		EngineGeneralDebug::MsgBoxError("Sound Load Error");
		return false;
	}

	if (nullptr == sound_)
	{
		EngineGeneralDebug::MsgBoxError("Sound Pointer nullptr Error");
		return false;
	}

	return true;
}