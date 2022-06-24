#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageGuileBack : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageGuileBack(); // default constructer ����Ʈ ������
	~StageGuileBack(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageGuileBack(const StageGuileBack& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageGuileBack(StageGuileBack&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageGuileBack& operator=(const StageGuileBack& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageGuileBack& operator=(const StageGuileBack&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

