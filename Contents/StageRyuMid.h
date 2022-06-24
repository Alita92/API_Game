#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageRyuMid : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRenderer_;


public:		
	StageRyuMid(); // default constructer ����Ʈ ������
	~StageRyuMid(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageRyuMid(const StageRyuMid& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageRyuMid(StageRyuMid&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageRyuMid& operator=(const StageRyuMid& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageRyuMid& operator=(const StageRyuMid&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void Update() override;
	void LastCheck() override;
};

