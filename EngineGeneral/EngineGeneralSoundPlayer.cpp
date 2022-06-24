#include "EngineGeneralSoundPlayer.h"
#include "EngineGeneralSoundFile.h"
#include "EngineGeneralDebug.h"

// Static Var
// Static Func

// constructer destructer
EngineGeneralSoundPlayer::EngineGeneralSoundPlayer()
	: playSoundFile_(nullptr)
	, playChannel_(nullptr), playCount_(-1)
{
}

EngineGeneralSoundPlayer::~EngineGeneralSoundPlayer()
{
}

EngineGeneralSoundPlayer::EngineGeneralSoundPlayer(EngineGeneralSoundPlayer&& _other) noexcept
	: playSoundFile_(_other.playSoundFile_)
	, playChannel_(_other.playChannel_), playCount_(_other.playCount_)
{
}

//member Func

bool EngineGeneralSoundPlayer::IsPlay()
{
	bool Check = false;
	// 이 FMOD를 짠사람이 그냥 이렇게 짠거에요.
	playChannel_->isPlaying(&Check);
	return Check;
}

void EngineGeneralSoundPlayer::PlayCountReset(int _Count /*= -1*/)
{
	playCount_ = _Count;
}

void EngineGeneralSoundPlayer::PlayOverlap(const std::string& _name, int _LoopCount/* = 1*/)
{
	EngineGeneralSoundFile* SoundPtr = EngineGeneralSound::GetEngineGeneralSound().FindSound(_name);

	if (nullptr == SoundPtr)
	{
		EngineGeneralDebug::MsgBoxError("PlaySound Error");
		return;
	}

	if (0 == playCount_)
	{
		return;
	}

	EngineGeneralSound::GetEngineGeneralSound().soundSystem_->playSound(
		SoundPtr->sound_
		, nullptr
		, false
		, &playChannel_);

	--playCount_;

	playChannel_->setLoopCount(_LoopCount);

}

void EngineGeneralSoundPlayer::PlayAlone(const std::string& _name, int _LoopCount /*= 1*/)
{
	// 함수를 만들어서 그함수를 다시 실행
	EngineGeneralSoundFile* SoundPtr = EngineGeneralSound::GetEngineGeneralSound().FindSound(_name);

	if (nullptr == SoundPtr)
	{
		EngineGeneralDebug::MsgBoxError("PlaySound Error");
		return;
	}

	if (true == IsPlay())
	{
		return;
	}

	if (0 == playCount_)
	{
		return;
	}

	EngineGeneralSound::GetEngineGeneralSound().soundSystem_->playSound(
		SoundPtr->sound_
		, nullptr
		, false
		, &playChannel_);

	--playCount_;

	playChannel_->setLoopCount(_LoopCount);
}

void EngineGeneralSoundPlayer::Stop()
{
	if (nullptr == playChannel_)
	{
		return;
	}

	playChannel_->stop();
	playChannel_ = nullptr;
}