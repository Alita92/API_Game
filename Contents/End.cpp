#include "End.h"
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
End::End()
{
	SetPos({EngineScreen::GetScreen().GetSize().halffloat4()});
	SetRenderOrder(1);
}

End::~End()
{
}

End::End(End&& _other) noexcept
{
}

//member Func
void End::Render()
{
	mainSpriteRender_->StartRender();
	//ImageAsset* importedImageBuffer = AssetManager::GetAssetManager().FindImportedImage("End.bmp");
	//ImageAsset* nextBufferScreen = AssetManager::GetAssetManager().GetNextBuffer();
	//nextBufferScreen->MyBitCopyToImageSizeToCenter(importedImageBuffer, GetPos());
}

void End::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("End.bmp");
}

void End::UpdateBefore()
{

}
void End::Update()
{
	if (true == InputManager::GetInputManager().IsButtonDown("Start"))
	{
		SceneManager::GetSceneManager().ChangeScene("TitleScene");
	}


}
void End::UpdateAfter()
{

}

