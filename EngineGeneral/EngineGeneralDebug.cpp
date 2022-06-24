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
void EngineGeneralDebug::GameDestroyer() // ��� �ǵ��� ũ���� �߻��� �� �Լ��� ����մϴ�.
{
	assert(false);
}

void EngineGeneralDebug::CheckMemoryLeak() // ��� ���� �Ҵ� �� üũ�� �� �Լ��� ����մϴ�.
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	
}

void EngineGeneralDebug::MsgBoxError(const char* _ptr)
{
	MessageBoxA(nullptr, _ptr, "Error", MB_OK);
	assert(false);
}