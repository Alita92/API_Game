#include "ResultBackground.h"
#include <EngineScreen.h>
#include <EngineGeneralTime.h>
#include <ImageAsset.h>
#include <InputManager.h>
#include <AssetManager.h>
#include <SceneManager.h>
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
ResultBackground::ResultBackground()
{
	SetPos({ EngineScreen::GetScreen().GetSize().halffloat4() });
	SetRenderOrder(1);
}

ResultBackground::~ResultBackground()
{
}

ResultBackground::ResultBackground(ResultBackground&& _other) noexcept
{
}

//member Func
void ResultBackground::UpdateBefore()
{
	if (-335 >= GetPos().y)
	{
		SetPos({ GetPos().x, 672 + 336 + 671 });
	}
}

void ResultBackground::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("ResultBackground.bmp");
}

void ResultBackground::Update()
{
	SetMove(float4::UP * 300.0f * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat());
}

void ResultBackground::UpdateAfter()
{

}

void ResultBackground::Render()
{
	mainSpriteRender_->StartRender();
}
