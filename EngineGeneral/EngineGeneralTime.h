#pragma once
#include <Windows.h>

// �з� :
// �뵵 : 
// ���� : ���� �ý��ۿ� ��ġ�� ��� �ð� ������ �ְ��մϴ�.

class EngineGeneralTime
{
private:	// member Var
	static EngineGeneralTime* engineGeneralTime_;

public:
	static EngineGeneralTime& GetEngineGeneralTime()
	{
		return *engineGeneralTime_;
	}

	static void DestroyMemory()
	{
		if (nullptr != engineGeneralTime_)
		{
			delete engineGeneralTime_;
			engineGeneralTime_ = nullptr;
		}
	}

private:
	LARGE_INTEGER timeCount_;
	LARGE_INTEGER startCheck_;
	LARGE_INTEGER endCheck_;
	double deltaTime_;

public:
	double GetDeltaTimeDouble()
	{
		return deltaTime_;
	}

	float GetDeltaTimeFloat()
	{
		return static_cast<float>(deltaTime_);
	}

private:		
	EngineGeneralTime(); // default constructer ����Ʈ ������
	~EngineGeneralTime(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralTime(const EngineGeneralTime& _other) = delete; // default Copy constructer ����Ʈ ���������
	EngineGeneralTime(const EngineGeneralTime&& _other); // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralTime& operator=(const EngineGeneralTime& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralTime& operator=(const EngineGeneralTime&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void ResetTime();
	void CheckTime();
};

