#include "StageGuileFore.h"
#include "FightScene.h"
#include <EngineScreen.h>
#include <EngineGeneralTime.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <SpriteRenderer.h>
#include <EngineGeneralMath.h>
#include <GameCollision.h>

// Static Var
// Static Func

// constructer destructer
StageGuileFore::StageGuileFore()
{
	SetPos({ 572,266 });
	SetRenderOrder(5);
}

StageGuileFore::~StageGuileFore()
{
}

StageGuileFore::StageGuileFore(StageGuileFore&& _other) noexcept
{
}

//member Func
void StageGuileFore::Render()
{
	mainSpriteRender_->StartRender();
}
void StageGuileFore::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageGuileFore.bmp");
}
void StageGuileFore::UpdateBefore()
{

}
void StageGuileFore::Update()
{

}
void StageGuileFore::UpdateAfter()
{

}