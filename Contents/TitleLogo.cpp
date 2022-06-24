#include "TitleLogo.h"
#include <EngineScreen.h>
#include <EngineGeneralTime.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <SceneManager.h>
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
TitleLogo::TitleLogo()
{
	SetPos({EngineScreen::GetScreen().GetSize().halffloat4()});
	SetRenderOrder(1);
}

TitleLogo::~TitleLogo()
{
}

TitleLogo::TitleLogo(TitleLogo&& _other) noexcept
{
}

void TitleLogo::LastCheck()
{
	//mainSpriteRender_ = CreateSpriteRenderer("TitleLogo.bmp");
	mainSpriteRender_ = CreateSpriteRenderer("TitleAnimation.bmp");
	mainSpriteRender_->CreateSpriteAnimation("TitleAnimation", "TitleAnimation.bmp", 0, 5, true, 0.102f);
	mainSpriteRender_->ChangeSpriteAnimation("TitleAnimation");

}

void TitleLogo::UpdateBefore()
{

}

void TitleLogo::Update()
{
	//SetMove(float4::RIGHT * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 100.0f);
}
void TitleLogo::UpdateAfter()
{

}
void TitleLogo::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
	//mainSpriteRender_->StartRender();
	//ImageAsset* importedImageBuffer = AssetManager::GetAssetManager().FindImportedImage("TitleLogo.bmp");
	//ImageAsset* nextBufferScreen = AssetManager::GetAssetManager().GetNextBuffer();
	//nextBufferScreen->MyBitCopyToImageSizeToCenter(importedImageBuffer,GetPos());
}