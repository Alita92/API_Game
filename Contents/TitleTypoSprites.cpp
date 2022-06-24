#include "TitleTypoSprites.h"
#include <EngineScreen.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <SpriteRenderer.h>


// Static Var
// Static Func

// constructer destructer
TitleTypoSprites::TitleTypoSprites()
{
	SetPos({ 576,563,0,1 }); // 측정된 위치
	SetRenderOrder(1);
}

TitleTypoSprites::~TitleTypoSprites()
{
}

TitleTypoSprites::TitleTypoSprites(TitleTypoSprites&& _other) noexcept
{
}

//member Func
void TitleTypoSprites::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("InsertCoin.bmp");
}

void TitleTypoSprites::Update()
{

}

void TitleTypoSprites::Render()
{
	mainSpriteRender_->StartRender();
}

void TitleTypoSprites::ChangeSprite()
{
	mainSpriteRender_->SetImageAsset("PushStart.bmp");
}