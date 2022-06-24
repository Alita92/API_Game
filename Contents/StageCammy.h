#pragma once
#include <Object.h>

// �з� :
// �뵵 :
// ���� :
class StageCammy : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageCammy(); // default constructer ����Ʈ ������
	~StageCammy(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageCammy(const StageCammy& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageCammy(StageCammy&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageCammy& operator=(const StageCammy& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageCammy& operator=(const StageCammy&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

	void AnimationInit();

};

