#pragma once
#include "EngineGeneralPath.h"

// �з� :
// �뵵 :
// ���� :
class EngineGeneralDirectory : public EngineGeneralPath
{
public:		
	EngineGeneralDirectory(); // default constructer ����Ʈ ������
	~EngineGeneralDirectory(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralDirectory(const EngineGeneralDirectory& _other); // default Copy constructer ����Ʈ ���������
	EngineGeneralDirectory(EngineGeneralDirectory&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralDirectory& operator=(const EngineGeneralDirectory& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralDirectory& operator=(const EngineGeneralDirectory&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	std::string GetDirectoryName();
	bool IsFolderExists();

public:
	bool IsReachedtoRoot(); // ��Ʈ ��ο� �����ߴ����� ����
	void MovetoParentFolder(); // 1�ܰ� �θ�(����) ������ �̵�
	bool MovetoParentFolder(std::string _dirName); // Ư�� ���ϸ��� �θ� ������ �̵�
	bool MovetoChildFolder(std::string _dirName); // Ư�� ���ϸ��� �ڽ�(����) ������ �̵�

public:
	std::string GetRelativePath(std::string _DirName); // �ϼ��� ��� ��� �����ϴ� �����Լ�
};

