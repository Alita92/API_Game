#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageRyu : public Object
{	// member Var
private:
	SpriteRenderer* foreSpriteRender_;


public:		
	StageRyu(); // default constructer ����Ʈ ������
	~StageRyu(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageRyu(const StageRyu& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageRyu(StageRyu&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageRyu& operator=(const StageRyu& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageRyu& operator=(const StageRyu&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����


public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

};

