#include "VSPortraitP2.h"
#include <EngineScreen.h>
#include <ImageAsset.h>
#include <AssetManager.h>

// Static Var
// Static Func

// constructer destructer
VSPortraitP2::VSPortraitP2()
{
	SetPos({ 824,280,0,1 });
	SetRenderOrder(10);
}

VSPortraitP2::~VSPortraitP2()
{
}

VSPortraitP2::VSPortraitP2(VSPortraitP2&& _other) noexcept
{
}

//member Func
void VSPortraitP2::Render()
{
	ImageAsset* importedImageBuffer = AssetManager::GetAssetManager().FindImportedImage("RPortraitCammy.bmp");
	ImageAsset* nextBufferScreen = AssetManager::GetAssetManager().GetNextBuffer();
	nextBufferScreen->MyBitCopyToImageSizeToCenter(importedImageBuffer, GetPos());
}