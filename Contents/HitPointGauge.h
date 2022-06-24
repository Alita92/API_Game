#pragma once
#include <Object.h>

// �з� :
// �뵵 :
// ���� :
class HitPointGauge : public Object
{
private:	// member Var
	SpriteRenderer* mainSpriteRender_;
public:		
	HitPointGauge(); // default constructer ����Ʈ ������
	~HitPointGauge(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	HitPointGauge(const HitPointGauge& _other) = delete; // default Copy constructer ����Ʈ ���������
	HitPointGauge(HitPointGauge&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	HitPointGauge& operator=(const HitPointGauge& _other) = delete; // default Copy operator ����Ʈ ���� ������
	HitPointGauge& operator=(const HitPointGauge&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Render() override;
	void LastCheck() override;
	void UpdateBefore() override;
	void Update() override;
	void UpdateAfter() override;
	void Collision() override;
};

