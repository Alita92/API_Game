#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class VSPortraitP2 : public Object
{
private:	// member Var

public:		
	VSPortraitP2(); // default constructer ����Ʈ ������
	~VSPortraitP2(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	VSPortraitP2(const VSPortraitP2& _other) = delete; // default Copy constructer ����Ʈ ���������
	VSPortraitP2(VSPortraitP2&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	VSPortraitP2& operator=(const VSPortraitP2& _other) = delete; // default Copy operator ����Ʈ ���� ������
	VSPortraitP2& operator=(const VSPortraitP2&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
};

