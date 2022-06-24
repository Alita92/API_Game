#include "SelectBackground.h"
#include <EngineScreen.h>
#include <EngineGeneralTime.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <SpriteRenderer.h>


// Static Var
// Static Func

// constructer destructer
SelectBackground::SelectBackground()
{
	SetPos({ EngineScreen::GetScreen().GetSize().halffloat4() });
	SetRenderOrder(3);
}

SelectBackground::~SelectBackground()
{
}

SelectBackground::SelectBackground(SelectBackground&& _other) noexcept
{
}

//member Func
void SelectBackground::Render()
{
	mainSpriteRender_->StartRender();
}

void SelectBackground::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("SelectBackground.bmp");
}
void SelectBackground::UpdateBefore() 
{

}
void SelectBackground::Update()
{
}
void SelectBackground::UpdateAfter()
{

}