#pragma once

// �з� :
// �뵵 :
// ���� :
class EngineGeneralDebug
{
private:	// member Var

public:		
	EngineGeneralDebug(); // default constructer ����Ʈ ������
	~EngineGeneralDebug(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralDebug(const EngineGeneralDebug& _other) = delete; // default Copy constructer ����Ʈ ���������
	EngineGeneralDebug(EngineGeneralDebug&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralDebug& operator=(const EngineGeneralDebug& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralDebug& operator=(const EngineGeneralDebug&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	static void GameDestroyer(); // ��� ���� �˴ٿ��� �� �Լ��� ����մϴ�.
	static void CheckMemoryLeak(); // ��� �� �޸� �� üũ�� �� �Լ��� ����մϴ�.
	static void MsgBoxError(const char* _ptr);
};

