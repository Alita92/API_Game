#include "WinnerTexth.h"
#include <EngineScreen.h>
#include "FightScene.h"
#include <EngineGeneralTime.h>
#include <ImageAsset.h>
#include <InputManager.h>
#include <AssetManager.h>
#include <SceneManager.h>
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
WinnerTexth::WinnerTexth()
{
	SetPos({576, 550});
	SetRenderOrder(10);
}

WinnerTexth::~WinnerTexth()
{
}

WinnerTexth::WinnerTexth(WinnerTexth&& _other) noexcept
{
}

//member Func
void WinnerTexth::UpdateBefore()
{

}
void WinnerTexth::Update()
{
	mainSpriteRenderer_->SetImageAsset(FightScene::winnerName_ + "WinText.bmp");
}
void WinnerTexth::UpdateAfter()
{

}
void WinnerTexth::Render()
{
	mainSpriteRenderer_->StartRender();
}
void WinnerTexth::LastCheck()
{
	mainSpriteRenderer_ = CreateSpriteRenderer("RyuWinText.bmp");
}