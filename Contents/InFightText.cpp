#include "InFightText.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>

InFightText::InFightText()
	: mainSpriteRender_(nullptr), interTime_(0.0f), isTextShown_(false)
{
	SetRenderOrder(11);
}

InFightText::~InFightText()
{
}

InFightText::InFightText(InFightText&& _other) noexcept
	: mainSpriteRender_(nullptr), interTime_(0.0f), isTextShown_(false)
{
}

void InFightText::SetText(std::string _name)
{
	mainSpriteRender_->SetImageAsset(_name + ".bmp");
}

//member Func
void InFightText::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("Combo.bmp");
	mainSpriteRender_->SetCameraEffectOff();
}

void InFightText::Update()
{
	if ("P1" == GetName())
	{
		if (280.0f > GetPos().x && false == isTextShown_) // ����
		{
			SetMove(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (280.0f <= GetPos().x) // ������ ���� ��ġ�� �ٴٸ� �� ���ð�
		{
			interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			isTextShown_ = true;
		}
		if (1.5f < interTime_ && true == isTextShown_) // ���ð� �ʰ� �� ����
		{
			SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (-280.0f > GetPos().x && true == isTextShown_) // ���� �� ����
		{
			Death();
		}
	}

	if ("P2" == GetName())
	{
		if (872.0f < GetPos().x && false == isTextShown_) // ����
		{
			SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (872.0f >= GetPos().x) // ������ ���ð�
		{
			interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			isTextShown_ = true;
		}
		if (1.5f < interTime_ && true == isTextShown_) // ���ð� �ʰ� �� ����
		{
			SetMove(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (1432.0f < GetPos().x && true == isTextShown_) // ���� �� ����
		{
			Death();
		}
	}

}

void InFightText::Render()
{
	mainSpriteRender_->StartRender();
}