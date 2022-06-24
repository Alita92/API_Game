#include "HPbar.h"
#include <SpriteRenderer.h>
#include <EngineGeneralMath.h>
#include "FightScene.h"
#include <EngineGeneralTime.h>
#include "CharacterHandle.h"
#include "Ryu.h"
#include <ImageAsset.h>
// Static Var
// Static Func

// constructer destructer
HPbar::HPbar()
	: mainSpriteRender_(nullptr), barMoveSpeed_(150.0f), p2debugFloat_(432), damagedBar_(0), p2HpDecliner_(0), parentCharacter_(nullptr), barLength_(432)
{
	SetRenderOrder(30);
}

HPbar::~HPbar()
{
}

HPbar::HPbar(HPbar&& _other) noexcept
	: mainSpriteRender_(_other.mainSpriteRender_), barMoveSpeed_(_other.barMoveSpeed_), p2debugFloat_(_other.p2debugFloat_), damagedBar_(_other.damagedBar_), p2HpDecliner_(_other.p2HpDecliner_), parentCharacter_(nullptr)
	, barLength_(432)
{
}

//member Func
void HPbar::Render()
{
	mainSpriteRender_->StartRender();
}

void HPbar::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("HPbar.bmp");
	mainSpriteRender_->SetCameraEffectOff();
}

void HPbar::UpdateBefore()
{

}

void HPbar::Update()
{

	HealthCheckUpdate();
}

void HPbar::UpdateAfter()
{

}

void HPbar::HealthCheckUpdate()
{
	float curHp = static_cast<float>(parentCharacter_->GetCurHp()); // ���� ü��
	float fixedHp = curHp * (static_cast<float>(barLength_) / parentCharacter_->originalHp_); // ���� ü���� ü�¹� x�� ��Ʈ���� �°� ����

	float fixedRemainHp = barLength_ - fixedHp; // x�� ��Ʈ���� �°� ���������� "�Ҹ�� ü��"
	if ("P1HPbar" == this->GetName())
	{
			this->SetPos({ originalPosX_ + fixedRemainHp , originalPosY_ }); // �Ҹ�� ü�¸�ŭ �������� ü�¹� �̵�
	}
	this->mainSpriteRender_->SetRenderSize({ barLength_ -fixedRemainHp , 21 }); // �Ҹ�� ü�¸�ŭ �������Ǵ� �̹����� �����ʺ��� �ڸ���

}