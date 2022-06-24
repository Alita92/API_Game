#include "VSPortraitP1.h"
#include <EngineScreen.h>
#include <ImageAsset.h>
#include <AssetManager.h>

// Static Var
// Static Func

// constructer destructer
VSPortraitP1::VSPortraitP1()
{
	SetPos({200,280,0,1});
	SetRenderOrder(10);
}

VSPortraitP1::~VSPortraitP1()
{
}

VSPortraitP1::VSPortraitP1(VSPortraitP1&& _other) noexcept
{
}

//member Func
void VSPortraitP1::Render()
{
	ImageAsset* importedImageBuffer = AssetManager::GetAssetManager().FindImportedImage("LPortraitRyu.bmp");
	ImageAsset* nextBufferScreen = AssetManager::GetAssetManager().GetNextBuffer();
	nextBufferScreen->MyBitCopyToImageSizeToCenter(importedImageBuffer, GetPos());
}