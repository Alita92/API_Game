#include "EngineGeneralDebug.h"
#include <assert.h>
#include <crtdbg.h>
#include <Windows.h>

// Static Var
// Static Func

// constructer destructer
EngineGeneralDebug::EngineGeneralDebug()
{
}

EngineGeneralDebug::~EngineGeneralDebug()
{
}

EngineGeneralDebug::EngineGeneralDebug(EngineGeneralDebug&& _other) noexcept
{
}
//member Func
void EngineGeneralDebug::GameDestroyer() // 모든 의도된 크래시 발생은 이 함수를 사용합니다.
{
	assert(false);
}

void EngineGeneralDebug::CheckMemoryLeak() // 모든 동적 할당 릭 체크는 이 함수를 사용합니다.
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	
}

void EngineGeneralDebug::MsgBoxError(const char* _ptr)
{
	MessageBoxA(nullptr, _ptr, "Error", MB_OK);
	assert(false);
}