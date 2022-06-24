#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class SelectBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	SelectBackground(); // default constructer ����Ʈ ������
	~SelectBackground(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	SelectBackground(const SelectBackground& _other) = delete; // default Copy constructer ����Ʈ ���������
	SelectBackground(SelectBackground&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	SelectBackground& operator=(const SelectBackground& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SelectBackground& operator=(const SelectBackground&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

