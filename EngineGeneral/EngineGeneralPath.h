#pragma once
#include <string>
// 분류 :
// 용도 :
// 설명 :
class EngineGeneralPath
{
protected:	// member Var
	std::string path_;

public:		
	EngineGeneralPath(); // default constructer 디폴트 생성자
	~EngineGeneralPath(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralPath(const EngineGeneralPath& _other); // default Copy constructer 디폴트 복사생성자
	EngineGeneralPath(EngineGeneralPath&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralPath& operator=(const EngineGeneralPath& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralPath& operator=(const EngineGeneralPath&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
};

