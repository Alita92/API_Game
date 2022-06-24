#include "FightBackGround.h"
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
FightBackGround::FightBackGround()
{
	SetPos({ EngineScreen::GetScreen().GetSize().halffloat4() });
	SetRenderOrder(1);
}

FightBackGround::~FightBackGround()
{
}

FightBackGround::FightBackGround(FightBackGround&& _other) noexcept
{
}

//member Func
void FightBackGround::Render()
{
	// mainSpriteRender_->StartRender();
	ImageAsset* importedImageBuffer = AssetManager::GetAssetManager().FindImportedImage("FightBackground.bmp");
	ImageAsset* nextBufferScreen = AssetManager::GetAssetManager().GetNextBuffer();
	nextBufferScreen->MyBitCopyToImageSizeToCenter(importedImageBuffer, GetCameraLinkedPos());
}
void FightBackGround::LastCheck()
{
	//mainSpriteRender_ = CreateSpriteRenderer("FightBackground.bmp");
}
void FightBackGround::UpdateBefore()
{

}
void FightBackGround::Update()
{

}
void FightBackGround::UpdateAfter()
{

}