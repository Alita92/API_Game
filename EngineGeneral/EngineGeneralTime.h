#pragma once
#include <Windows.h>

// 분류 :
// 용도 : 
// 설명 : 게임 시스템에 미치는 모든 시간 개념을 주관합니다.

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
	EngineGeneralTime(); // default constructer 디폴트 생성자
	~EngineGeneralTime(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralTime(const EngineGeneralTime& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EngineGeneralTime(const EngineGeneralTime&& _other); // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralTime& operator=(const EngineGeneralTime& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralTime& operator=(const EngineGeneralTime&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void ResetTime();
	void CheckTime();
};

