#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class SelectVSBackground : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	SelectVSBackground(); // default constructer ����Ʈ ������
	~SelectVSBackground(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	SelectVSBackground(const SelectVSBackground& _other) = delete; // default Copy constructer ����Ʈ ���������
	SelectVSBackground(SelectVSBackground&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	SelectVSBackground& operator=(const SelectVSBackground& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SelectVSBackground& operator=(const SelectVSBackground&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

