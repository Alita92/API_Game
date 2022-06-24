#pragma once
#include "EngineGeneralPath.h"

// 분류 :
// 용도 :
// 설명 :
class EngineGeneralDirectory : public EngineGeneralPath
{
public:		
	EngineGeneralDirectory(); // default constructer 디폴트 생성자
	~EngineGeneralDirectory(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralDirectory(const EngineGeneralDirectory& _other); // default Copy constructer 디폴트 복사생성자
	EngineGeneralDirectory(EngineGeneralDirectory&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralDirectory& operator=(const EngineGeneralDirectory& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralDirectory& operator=(const EngineGeneralDirectory&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	std::string GetDirectoryName();
	bool IsFolderExists();

public:
	bool IsReachedtoRoot(); // 루트 경로에 도달했는지를 리턴
	void MovetoParentFolder(); // 1단계 부모(상위) 폴더로 이동
	bool MovetoParentFolder(std::string _dirName); // 특정 파일명의 부모 폴더로 이동
	bool MovetoChildFolder(std::string _dirName); // 특정 파일명의 자식(하위) 폴더로 이동

public:
	std::string GetRelativePath(std::string _DirName); // 완성된 상대 경로 리턴하는 최종함수
};

