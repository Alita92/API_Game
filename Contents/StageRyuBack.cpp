#include "StageRyuBack.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralMath.h>


// Static Var
// Static Func

// constructer destructer
StageRyuBack::StageRyuBack()
{
	SetPos({ 1400,276 });
	SetRenderOrder(1);
}

StageRyuBack::~StageRyuBack()
{
}

StageRyuBack::StageRyuBack(StageRyuBack&& _other) noexcept
{
}

//member Func
void StageRyuBack::Render()
{
	mainSpriteRenderer_->StartRender();
}

void StageRyuBack::LastCheck()
{
	mainSpriteRenderer_ = CreateSpriteRenderer("StageRyuFog.bmp");
	mainSpriteRenderer_->SetCameraEffectOff();

}

void StageRyuBack::Update()
{
	SetMove(float4::LEFT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 15.0f);
}