#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageCammyBack : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageCammyBack(); // default constructer ����Ʈ ������
	~StageCammyBack(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageCammyBack(const StageCammyBack& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageCammyBack(StageCammyBack&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageCammyBack& operator=(const StageCammyBack& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageCammyBack& operator=(const StageCammyBack&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

