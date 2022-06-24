#include "StageGuileMid.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralMath.h>

// Static Var
// Static Func

// constructer destructer
StageGuileMid::StageGuileMid()
{
	SetPos({ 572,266 });
	SetRenderOrder(4);
}

StageGuileMid::~StageGuileMid()
{
}

StageGuileMid::StageGuileMid(StageGuileMid&& _other) noexcept
{
}

//member Func
void StageGuileMid::Render()
{
	mainSpriteRender_->StartRender();
}

void StageGuileMid::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageGuileMid.bmp");
	mainSpriteRender_->SetCameraEffectOff();
}

void StageGuileMid::Update()
{

}