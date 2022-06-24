#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageGuileFore : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageGuileFore(); // default constructer ����Ʈ ������
	~StageGuileFore(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageGuileFore(const StageGuileFore& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageGuileFore(StageGuileFore&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageGuileFore& operator=(const StageGuileFore& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageGuileFore& operator=(const StageGuileFore&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

