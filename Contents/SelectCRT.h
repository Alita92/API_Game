#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class SelectCRT : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	SelectCRT(); // default constructer ����Ʈ ������
	~SelectCRT(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	SelectCRT(const SelectCRT& _other) = delete; // default Copy constructer ����Ʈ ���������
	SelectCRT(SelectCRT&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	SelectCRT& operator=(const SelectCRT& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SelectCRT& operator=(const SelectCRT&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
};

