#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageChunLiFore : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageChunLiFore(); // default constructer ����Ʈ ������
	~StageChunLiFore(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageChunLiFore(const StageChunLiFore& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageChunLiFore(StageChunLiFore&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageChunLiFore& operator=(const StageChunLiFore& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageChunLiFore& operator=(const StageChunLiFore&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;

};

