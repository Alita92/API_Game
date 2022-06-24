#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class ContinueBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	ContinueBackground(); // default constructer ����Ʈ ������
	~ContinueBackground(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	ContinueBackground(const ContinueBackground& _other) = delete; // default Copy constructer ����Ʈ ���������
	ContinueBackground(ContinueBackground&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	ContinueBackground& operator=(const ContinueBackground& _other) = delete; // default Copy operator ����Ʈ ���� ������
	ContinueBackground& operator=(const ContinueBackground&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Update() override;
	void Render() override;
	void LastCheck() override;
};

