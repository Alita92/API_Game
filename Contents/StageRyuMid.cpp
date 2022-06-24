#include "StageRyuMid.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralMath.h>



// Static Var
// Static Func

// constructer destructer
StageRyuMid::StageRyuMid()
{
	SetPos({ 572,266 });
	SetRenderOrder(2);
}

StageRyuMid::~StageRyuMid()
{
}

StageRyuMid::StageRyuMid(StageRyuMid&& _other) noexcept
{
}

//member Func
void StageRyuMid::Render()
{
	mainSpriteRenderer_->StartRender();
}

void StageRyuMid::LastCheck()
{
	mainSpriteRenderer_ = CreateSpriteRenderer("StageRyuMid.bmp");
	mainSpriteRenderer_->SetCameraEffectOff();
}

void StageRyuMid::Update()
{

}