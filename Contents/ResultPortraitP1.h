#pragma once

// �з� :
// �뵵 :
// ���� :
class ResultPortraitP1
{
private:	// member Var

public:		
	ResultPortraitP1(); // default constructer ����Ʈ ������
	~ResultPortraitP1(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ResultPortraitP1(const ResultPortraitP1& _other) = delete; // default Copy constructer ����Ʈ ���������
	ResultPortraitP1(ResultPortraitP1&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ResultPortraitP1& operator=(const ResultPortraitP1& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ResultPortraitP1& operator=(const ResultPortraitP1&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

