#pragma once
#include <Object.h>

// �з� :
// �뵵 :
// ���� :
class VSPortraitP1 : public Object
{
private:	// member Var

public:		
	VSPortraitP1(); // default constructer ����Ʈ ������
	~VSPortraitP1(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	VSPortraitP1(const VSPortraitP1& _other) = delete; // default Copy constructer ����Ʈ ���������
	VSPortraitP1(VSPortraitP1&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	VSPortraitP1& operator=(const VSPortraitP1& _other) = delete; // default Copy operator ����Ʈ ���� ������
	VSPortraitP1& operator=(const VSPortraitP1&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
};

