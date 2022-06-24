#include "EngineGeneralTime.h"


// Static Var
EngineGeneralTime* EngineGeneralTime::engineGeneralTime_ = new EngineGeneralTime();
// Static Func

// constructer destructer
EngineGeneralTime::EngineGeneralTime()
{
	ResetTime();
}

EngineGeneralTime::~EngineGeneralTime()
{
}

EngineGeneralTime::EngineGeneralTime(const EngineGeneralTime&& _other)
	: timeCount_(_other.timeCount_),
	startCheck_(_other.startCheck_),
	endCheck_(_other.endCheck_)
{
}

//member Func
void EngineGeneralTime::ResetTime()
{
	QueryPerformanceFrequency(&timeCount_);
	QueryPerformanceCounter(&startCheck_);
	QueryPerformanceCounter(&endCheck_);
}

void EngineGeneralTime::CheckTime()
{
	QueryPerformanceCounter(&endCheck_);
	deltaTime_ = static_cast<double>((endCheck_.QuadPart - startCheck_.QuadPart)) / static_cast<double>(timeCount_.QuadPart);
	startCheck_.QuadPart = endCheck_.QuadPart;
}
