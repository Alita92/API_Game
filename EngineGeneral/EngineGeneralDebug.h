#pragma once

// 분류 :
// 용도 :
// 설명 :
class EngineGeneralDebug
{
private:	// member Var

public:		
	EngineGeneralDebug(); // default constructer 디폴트 생성자
	~EngineGeneralDebug(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralDebug(const EngineGeneralDebug& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EngineGeneralDebug(EngineGeneralDebug&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralDebug& operator=(const EngineGeneralDebug& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralDebug& operator=(const EngineGeneralDebug&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	static void GameDestroyer(); // 모든 강제 셧다운은 이 함수를 사용합니다.
	static void CheckMemoryLeak(); // 모든 힙 메모리 릭 체크는 이 함수를 사용합니다.
	static void MsgBoxError(const char* _ptr);
};

