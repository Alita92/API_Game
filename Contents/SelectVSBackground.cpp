#include "SelectVSBackground.h"
#include <SpriteRenderer.h>
#include <EngineScreen.h>
#include <EngineGeneralTime.h>
// Static Var
// Static Func

// constructer destructer
SelectVSBackground::SelectVSBackground()
{
	SetRenderOrder(50);
}

SelectVSBackground::~SelectVSBackground()
{
}

SelectVSBackground::SelectVSBackground(SelectVSBackground&& _other) noexcept
{
}

//member Func
void SelectVSBackground::Render()
{
	if (true == IsOn())
	{
		mainSpriteRender_->StartRender();
	}
}
void SelectVSBackground::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("SelectVSBackground.bmp");
}
void SelectVSBackground::UpdateBefore()
{
	if (true == IsOn())
	{
		if ( -335 >= GetPos().y)
		{
			SetPos({ GetPos().x, 672 + 336 + 671 });
		}
	}
}
void SelectVSBackground::Update()
{
	if (true == IsOn())
	{
		SetMove(float4::UP * 300.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
	}
}
void SelectVSBackground::UpdateAfter()
{

}