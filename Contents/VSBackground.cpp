#include "VSBackground.h"
#include <EngineScreen.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <SpriteRenderer.h>
#include <SceneManager.h>
#include <InputManager.h>

// Static Var
// Static Func

// constructer destructer
VSBackground::VSBackground()
{
	SetPos({EngineScreen::GetScreen().GetSize().halffloat4()});
	SetRenderOrder(1);
}

VSBackground::~VSBackground()
{
}

VSBackground::VSBackground(VSBackground&& _other) noexcept
{
}

void VSBackground::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("VSBackground.bmp");
}

//member Func
void VSBackground::Render()
{
	//ImageAsset* importedImageBuffer = AssetManager::GetAssetManager().FindImportedImage("VSBackground.bmp");
	//ImageAsset* nextBufferScreen = AssetManager::GetAssetManager().GetNextBuffer();
	//nextBufferScreen->MyBitCopyToImageSizeToCenter(importedImageBuffer, GetPos());
	mainSpriteRender_->StartRender();
}

void VSBackground::Update()
{
	if (true == InputManager::GetInputManager().IsButtonDown("Start"))
	{
		SceneManager::GetSceneManager().ChangeScene("FightScene");
	}
}
