#include "ScreenFade.h"
#include <SpriteRenderer.h>
#include <EngineScreen.h>
#include <EngineGeneralTime.h>


// Static Var
// Static Func

// constructer destructer
ScreenFade::ScreenFade()
	: alpha_(255.0f), mainSpriteRenderer_(nullptr), fadeEnum_(FadeEnum::MAX)
{
}

ScreenFade::~ScreenFade()
{
}

ScreenFade::ScreenFade(ScreenFade&& _other) noexcept
	: alpha_(_other.alpha_), mainSpriteRenderer_(_other.mainSpriteRenderer_), fadeEnum_(_other.fadeEnum_)
{
}

//member Func
void ScreenFade::FadeIn()
{
	fadeEnum_ = FadeEnum::FADEIN;
	alpha_ = 0.0f;
}

void ScreenFade::FadeOut()
{
	fadeEnum_ = FadeEnum::FADEOUT;
	alpha_ = 255.0f;
}

void ScreenFade::ChangeState(FadeEnum _state)
{

}

void ScreenFade::LastCheck()
{
	SetRenderOrder(451);
	mainSpriteRenderer_ = CreateSpriteRenderer("ScreenFade.bmp");
	mainSpriteRenderer_->SetPivotPos(EngineScreen::GetScreen().GetSize().halffloat4());
	mainSpriteRenderer_->SetCameraEffectOff();
	fadeEnum_ = FadeEnum::FADEOUT;

}

void ScreenFade::Update()
{
	switch (fadeEnum_)
	{
	case ScreenFade::FadeEnum::FADEIN:
		alpha_ += 150.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		mainSpriteRenderer_->SetAlpha(static_cast<int>(alpha_));
		
		break;
	case ScreenFade::FadeEnum::FADEOUT:
		alpha_ -= 150.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		mainSpriteRenderer_->SetAlpha(static_cast<int>(alpha_));

		break;
	case ScreenFade::FadeEnum::MAX:
		break;
	default:
		break;
	}
}

void ScreenFade::Render()
{
	mainSpriteRenderer_->StartRender();
}

