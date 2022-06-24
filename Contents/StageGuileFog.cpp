#include "StageGuileFog.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralMath.h>

// Static Var
// Static Func

// constructer destructer
StageGuileFog::StageGuileFog()
{
	SetPos({ 1400,276 });
	SetRenderOrder(2);
}

StageGuileFog::~StageGuileFog()
{
}

StageGuileFog::StageGuileFog(StageGuileFog&& _other) noexcept
{
}

//member Func
void StageGuileFog::Render()
{
	mainSpriteRender_->StartRender();
}

void StageGuileFog::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageGuileFog.bmp");
	mainSpriteRender_->SetCameraEffectOff();

}

void StageGuileFog::Update()
{
	SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 15.0f);
}