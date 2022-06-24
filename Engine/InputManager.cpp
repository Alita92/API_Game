#include "InputManager.h"
#include "EngineScreen.h"
#include <EngineGeneralDebug.h>

// Static Var
InputManager* InputManager::inputManager_ = new InputManager();
// Static Func

// constructer destructer
InputManager::InputKey::InputKey()
	: buttonDown_(false), buttonPress_(false), buttonUp_(false), buttonFree_(true), buttonKey_(-1)
{

}

InputManager::InputKey::~InputKey()
{

}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
	std::map<std::string, InputKey*>::iterator startIter = mapKeys_.begin();
	std::map<std::string, InputKey*>::iterator endIter = mapKeys_.end();

	for (; startIter != endIter; ++startIter)
	{
		if (nullptr == startIter->second)
		{
			continue;
		}

		delete startIter->second;
		startIter->second = nullptr;
	}

	mapKeys_.clear();
}

void InputManager::InputKey::InputKeyUpdate()
{
	if (0 != GetAsyncKeyState(buttonKey_))
	{
		if (false == buttonPress_)
		{
			buttonDown_ = true;
			buttonPress_ = true;
			buttonUp_= false;
			buttonFree_ = false;
		}
		// 이전에 눌렸다.
		else if (true == buttonPress_)
		{
			buttonDown_ = false;
			buttonPress_ = true;
			buttonUp_ = false;
			buttonFree_ = false;
		}
	}
	else
	{
		// 방금전까지도 키가 눌려있었다.
		if (true == buttonPress_)
		{
			buttonDown_ = false;
			buttonPress_ = false;
			buttonUp_ = true;
			buttonFree_ = false;
		}
		else if (false == buttonPress_)
		{
			// 최초 상태로 돌아온다.
			buttonDown_ = false;
			buttonPress_ = false;
			buttonUp_ = false;
			buttonFree_ = true;
		}
	}
}

InputManager::InputManager(InputManager&& _other) noexcept
{
}

//member Func
InputManager::InputKey* InputManager::FindButtonKey(const std::string _name)
{
	std::map<std::string, InputKey*>::iterator finder = mapKeys_.find(_name);

	if (finder == mapKeys_.end())
	{
		return nullptr;
	}

	return finder->second;
}

bool InputManager::CreateInputKey(const std::string _name, int _key, bool _isMoveKey)
{
	if ('a' <= _key && 'z' >= _key)
	{
		_key = std::toupper(_key);
	}

	InputKey* newKeyBuffer = new InputKey();
	newKeyBuffer->buttonKey_ = _key;
	newKeyBuffer->isMoveKey_ = _isMoveKey;
	newKeyBuffer->SetName(_name);
	
	mapKeys_.insert(std::map<std::string, InputKey*>::value_type(_name, newKeyBuffer));
	return true;
}

bool InputManager::IsButtonDown(const std::string _name)
{
	InputKey* keyPtrBuffer = FindButtonKey(_name);

	if (nullptr == keyPtrBuffer)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	return keyPtrBuffer->buttonDown_;
}

bool InputManager::IsButtonUp(const std::string _name)
{
	InputKey* keyPtrBuffer = FindButtonKey(_name);

	if (nullptr == keyPtrBuffer)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	return keyPtrBuffer->buttonUp_;
}
bool InputManager::IsButtonPress(const std::string _name)
{
	InputKey* keyPtrBuffer = FindButtonKey(_name);

	if (nullptr == keyPtrBuffer)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	return keyPtrBuffer->buttonPress_;
}
bool InputManager::IsButtonFree(const std::string _name)
{
	InputKey* keyPtrBuffer = FindButtonKey(_name);

	if (nullptr == keyPtrBuffer)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	return keyPtrBuffer->buttonFree_;
}

bool InputManager::IsButtonMove(const std::string _name)
{
	InputKey* keyPtrBuffer = FindButtonKey(_name);

	if (nullptr == keyPtrBuffer)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	return keyPtrBuffer->isMoveKey_;

}

bool InputManager::IsButtonKey(const std::string _name)
{
	InputKey* keyPtrBuffer = FindButtonKey(_name);

	if (nullptr == keyPtrBuffer)
	{
		return false;
	}

	return true;
}


void InputManager::InputUpdate()
{
	std::map<std::string, InputKey*>::iterator startIter = mapKeys_.begin();
	std::map<std::string, InputKey*>::iterator endIter = mapKeys_.end();

	for (; startIter != endIter; ++startIter)
	{
		startIter->second->InputKeyUpdate();
	}
}