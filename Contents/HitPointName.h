#pragma once
#include <Object.h>
// �з� :
// �뵵 :
// ���� :
class HitPointName : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;

public:		
	HitPointName(); // default constructer ����Ʈ ������
	~HitPointName(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	HitPointName(const HitPointName& _other) = delete; // default Copy constructer ����Ʈ ���������
	HitPointName(HitPointName&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	HitPointName& operator=(const HitPointName& _other) = delete; // default Copy operator ����Ʈ ���� ������
	HitPointName& operator=(const HitPointName&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;

public:
	void AnimationInit();
	void SetHitPointName(std::string _name);
};

