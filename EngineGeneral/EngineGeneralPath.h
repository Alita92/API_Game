#pragma once
#include <string>
// �з� :
// �뵵 :
// ���� :
class EngineGeneralPath
{
protected:	// member Var
	std::string path_;

public:		
	EngineGeneralPath(); // default constructer ����Ʈ ������
	~EngineGeneralPath(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralPath(const EngineGeneralPath& _other); // default Copy constructer ����Ʈ ���������
	EngineGeneralPath(EngineGeneralPath&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralPath& operator=(const EngineGeneralPath& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralPath& operator=(const EngineGeneralPath&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

