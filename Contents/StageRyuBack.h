#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageRyuBack : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;

public:		
	StageRyuBack(); // default constructer ����Ʈ ������
	~StageRyuBack(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageRyuBack(const StageRyuBack& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageRyuBack(StageRyuBack&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageRyuBack& operator=(const StageRyuBack& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageRyuBack& operator=(const StageRyuBack&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;

};

