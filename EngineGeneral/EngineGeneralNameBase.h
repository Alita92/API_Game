#pragma once
#include <string>
#include "EngineGeneralObjectBase.h"

class EngineGeneralNameBase : public EngineGeneralObjectBase
{
private:
	std::string name_;

public:
	std::string GetName()
	{
		return name_;
	}

	void SetName(const std::string& _name)
	{
		name_ = _name;
	}

public:
	virtual ~EngineGeneralNameBase()
	{

	}
};

