#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class StageGuileFog : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	StageGuileFog(); // default constructer ����Ʈ ������
	~StageGuileFog(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	StageGuileFog(const StageGuileFog& _other) = delete; // default Copy constructer ����Ʈ ���������
	StageGuileFog(StageGuileFog&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	StageGuileFog& operator=(const StageGuileFog& _other) = delete; // default Copy operator ����Ʈ ���� ������
	StageGuileFog& operator=(const StageGuileFog&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void Update() override;
};

