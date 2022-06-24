#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� : 
class FightBackGround : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	FightBackGround(); // default constructer ����Ʈ ������
	~FightBackGround(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	FightBackGround(const FightBackGround& _other) = delete; // default Copy constructer ����Ʈ ���������
	FightBackGround(FightBackGround&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	FightBackGround& operator=(const FightBackGround& _other) = delete; // default Copy operator ����Ʈ ���� ������
	FightBackGround& operator=(const FightBackGround&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
};

