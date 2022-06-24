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
		if (280.0f > GetPos().x && false == isTextShown_) // 등장
		{
			SetMove(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (280.0f <= GetPos().x) // 등장후 일정 위치에 다다를 시 대기시간
		{
			interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			isTextShown_ = true;
		}
		if (1.5f < interTime_ && true == isTextShown_) // 대기시간 초과 시 퇴장
		{
			SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (-280.0f > GetPos().x && true == isTextShown_) // 퇴장 후 삭제
		{
			Death();
		}
	}

	if ("P2" == GetName())
	{
		if (872.0f < GetPos().x && false == isTextShown_) // 등장
		{
			SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (872.0f >= GetPos().x) // 등장후 대기시간
		{
			interTime_ += EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
			isTextShown_ = true;
		}
		if (1.5f < interTime_ && true == isTextShown_) // 대기시간 초과 시 퇴장
		{
			SetMove(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 700.0f);
		}
		if (1432.0f < GetPos().x && true == isTextShown_) // 퇴장 후 삭제
		{
			Death();
		}
	}

}

void InFightText::Render()
{
	mainSpriteRender_->StartRender();
}