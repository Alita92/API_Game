#include "StageCammy.h"
#include "FightScene.h"
#include <EngineScreen.h>
#include <EngineGeneralTime.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <SpriteRenderer.h>
#include <EngineGeneralMath.h>

StageCammy::StageCammy()
{
	SetPos({ 572,231 });
	SetRenderOrder(5);
}

StageCammy::~StageCammy()
{
}

StageCammy::StageCammy(StageCammy&& _other) noexcept
{
}

void StageCammy::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}

void StageCammy::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageCammy.bmp");
	mainSpriteRender_->CreateSpriteAnimation("StageCammy", "StageCammy.bmp", 0, 5, true, 0.17f);
	mainSpriteRender_->ChangeSpriteAnimation("StageCammy");
}

void StageCammy::LastCheck()
{
	AnimationInit();
}
void StageCammy::UpdateBefore()
{

}
void StageCammy::Update()
{

}
void StageCammy::UpdateAfter()
{

}