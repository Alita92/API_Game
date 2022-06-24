#include "EngineGeneralPath.h"


// Static Var
// Static Func

// constructer destructer
EngineGeneralPath::EngineGeneralPath()
	: path_("")
{
}

EngineGeneralPath::~EngineGeneralPath()
{
}

EngineGeneralPath::EngineGeneralPath(const EngineGeneralPath& _other)
	:path_(_other.path_)
{
	
}

EngineGeneralPath::EngineGeneralPath(EngineGeneralPath&& _other) noexcept
{
}

//member Func