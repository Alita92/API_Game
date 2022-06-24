#include "VSTypo.h"
#include <EngineScreen.h>
#include <SpriteRenderer.h>
// Static Var
// Static Func

// constructer destructer
VSTypo::VSTypo()
{
	SetPos({ EngineScreen::GetScreen().GetSize().halffloat4() });
	SetRenderOrder(51);
}

VSTypo::~VSTypo()
{
}

VSTypo::VSTypo(VSTypo&& _other) noexcept
{
}

//member Func
void VSTypo::Render()
{
	if (true == IsOn())
	{
		mainSpriteRender_->StartRender();
	}
}
void VSTypo::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("VSTypo.bmp");
}