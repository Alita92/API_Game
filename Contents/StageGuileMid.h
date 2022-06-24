#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageGuileMid : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	StageGuileMid(); // default constructer ����Ʈ ������
	~StageGuileMid(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageGuileMid(const StageGuileMid& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageGuileMid(StageGuileMid&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageGuileMid& operator=(const StageGuileMid& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageGuileMid& operator=(const StageGuileMid&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void Update() override;
	void LastCheck() override;
};

