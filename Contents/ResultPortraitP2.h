#pragma once

// �з� :
// �뵵 :
// ���� :
class ResultPortraitP2
{
private:	// member Var

public:		
	ResultPortraitP2(); // default constructer ����Ʈ ������
	~ResultPortraitP2(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ResultPortraitP2(const ResultPortraitP2& _other) = delete; // default Copy constructer ����Ʈ ���������
	ResultPortraitP2(ResultPortraitP2&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ResultPortraitP2& operator=(const ResultPortraitP2& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ResultPortraitP2& operator=(const ResultPortraitP2&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

