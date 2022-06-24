#include "StageChunLiBack.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>
#include <EngineGeneralMath.h>

// Static Var
// Static Func

// constructer destructer
StageChunLiBack::StageChunLiBack()
{
	SetPos({ 576,336 });
	SetRenderOrder(2);
}

StageChunLiBack::~StageChunLiBack()
{
}

StageChunLiBack::StageChunLiBack(StageChunLiBack&& _other) noexcept
{
}

void StageChunLiBack::Render()
{
	mainSpriteRender_->StartRender();
}

void StageChunLiBack::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageChunLiBack.bmp");
	mainSpriteRender_->SetCameraEffectOff();

}

void StageChunLiBack::Update()
{

}