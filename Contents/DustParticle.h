#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class DustParticle : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	DustParticle(); // default constructer ����Ʈ ������
	~DustParticle(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	DustParticle(const DustParticle& _other) = delete; // default Copy constructer ����Ʈ ���������
	DustParticle(DustParticle&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	DustParticle& operator=(const DustParticle& _other) = delete; // default Copy operator ����Ʈ ���� ������
	DustParticle& operator=(const DustParticle&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void LastCheck() override;
	void Update() override;
	void Render() override;
};

