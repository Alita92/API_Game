#pragma once
#include <Object.h>
// �з� : VS Scene �� ��� ȭ���Դϴ�.
// �뵵 : �� ����� �ܻ����� ����ݴϴ�.
// ���� : RGB 0,0,170

class VSBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	VSBackground(); // default constructer ����Ʈ ������
	~VSBackground(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	VSBackground(const VSBackground& _other) = delete; // default Copy constructer ����Ʈ ���������
	VSBackground(VSBackground&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	VSBackground& operator=(const VSBackground& _other) = delete; // default Copy operator ����Ʈ ���� ������
	VSBackground& operator=(const VSBackground&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

