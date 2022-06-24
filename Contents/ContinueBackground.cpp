#include "ContinueBackground.h"
#include <SpriteRenderer.h>
#include <EngineScreen.h>
// Static Var
// Static Func

// constructer destructer
ContinueBackground::ContinueBackground()
{
	SetPos(EngineScreen::GetScreen().GetSize().halffloat4());
	SetRenderOrder(20);
}

ContinueBackground::~ContinueBackground()
{
}

ContinueBackground::ContinueBackground(ContinueBackground&& _other) noexcept
{
}

//member Func
void ContinueBackground::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("ContinueBackground.bmp");
}
void ContinueBackground::Update()
{

}
void ContinueBackground::Render()
{
	if (true == IsOn())
	{
		mainSpriteRender_->StartRender();
	}
}
