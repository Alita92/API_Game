#include "EngineGeneralObjectBase.h"


// Static Var
// Static Func

// constructer destructer
EngineGeneralObjectBase::EngineGeneralObjectBase()
	: isDeath_(false),
	isUpdate_(true),
	parent_(nullptr)
{
}

EngineGeneralObjectBase::~EngineGeneralObjectBase()
{
}

EngineGeneralObjectBase::EngineGeneralObjectBase(EngineGeneralObjectBase&& _other) noexcept
{
}

//member Func