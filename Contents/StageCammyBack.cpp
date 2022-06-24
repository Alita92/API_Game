#include "StageCammyBack.h"
#include <SpriteRenderer.h>
#include <EngineScreen.h>
// Static Var
// Static Func

// constructer destructer
StageCammyBack::StageCammyBack()
{
	SetPos(EngineScreen::GetScreen().GetSize().halffloat4());
	SetRenderOrder(2);
}

StageCammyBack::~StageCammyBack()
{
}

StageCammyBack::StageCammyBack(StageCammyBack&& _other) noexcept
{
}

//member Func
void StageCammyBack::Render()
{
	mainSpriteRender_->StartRender();
}
void StageCammyBack::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageCammyBack.bmp");
	mainSpriteRender_->SetCameraEffectOff();
}
void StageCammyBack::Update()
{

}