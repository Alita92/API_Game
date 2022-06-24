#include "RefereeText.h"
#include "FightScene.h"
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
RefereeText::RefereeText()
{
	SetRenderOrder(30);
}

RefereeText::~RefereeText()
{
}

RefereeText::RefereeText(RefereeText&& _other) noexcept
{
}

//member Func
void RefereeText::Render()
{
	if (true == IsOn())
	{
		mainSpriteRenderer_->StartRender();
	}

}
void RefereeText::LastCheck()
{
	mainSpriteRenderer_ = CreateSpriteRenderer("R1.bmp");
	mainSpriteRenderer_ = CreateSpriteRenderer("R2.bmp");
	mainSpriteRenderer_ = CreateSpriteRenderer("Fight.bmp");
	mainSpriteRenderer_->SetCameraEffectOff();
	Off();
}

void RefereeText::ChangeText(std::string _imageName)
{
	mainSpriteRenderer_->SetImageAsset(_imageName);
}

void RefereeText::UpdateBefore()
{

}

void RefereeText::Update()
{


}

void RefereeText::UpdateAfter()
{

}