#include "SelectCRT.h"
#include <EngineScreen.h>
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
SelectCRT::SelectCRT()
{
	SetPos(EngineScreen::GetScreen().GetSize().halffloat4());
	SetRenderOrder(200);
}

SelectCRT::~SelectCRT()
{
}

SelectCRT::SelectCRT(SelectCRT&& _other) noexcept
{
}

//member Func
void SelectCRT::Render()
{
	mainSpriteRender_->StartRender();
}

void SelectCRT::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("CRT.bmp");
	mainSpriteRender_->SetCameraEffectOff();
}