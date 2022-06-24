#include "StageGuileBack.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralMath.h>

// Static Var
// Static Func

// constructer destructer
StageGuileBack::StageGuileBack()
{
	SetPos({576,336 });
	SetRenderOrder(2);
}

StageGuileBack::~StageGuileBack()
{
}

StageGuileBack::StageGuileBack(StageGuileBack&& _other) noexcept
{
}

//member Func
void StageGuileBack::Render()
{
	mainSpriteRender_->StartRender();
}

void StageGuileBack::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageGuileBack.bmp");

}

void StageGuileBack::Update()
{

}