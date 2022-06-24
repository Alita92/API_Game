#include "StageChunLiFore.h"
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
StageChunLiFore::StageChunLiFore()
{
	SetPos({ 572,266 });
	SetRenderOrder(5);
}

StageChunLiFore::~StageChunLiFore()
{
}

StageChunLiFore::StageChunLiFore(StageChunLiFore&& _other) noexcept
{
}

//member Func
void StageChunLiFore::Render()
{
	mainSpriteRender_->StartRender();
}
void StageChunLiFore::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageChunLiFore.bmp");
}
void StageChunLiFore::UpdateBefore()
{

}
void StageChunLiFore::Update()
{

}
void StageChunLiFore::UpdateAfter()
{

}