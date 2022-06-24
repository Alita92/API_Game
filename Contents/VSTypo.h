#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class VSTypo : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	VSTypo(); // default constructer ����Ʈ ������
	~VSTypo(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	VSTypo(const VSTypo& _other) = delete; // default Copy constructer ����Ʈ ���������
	VSTypo(VSTypo&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	VSTypo& operator=(const VSTypo& _other) = delete; // default Copy operator ����Ʈ ���� ������
	VSTypo& operator=(const VSTypo&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
};

