#include "StageRyu.h"
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
StageRyu::StageRyu()
{
	SetPos({572,266});
	SetRenderOrder(3);
}

StageRyu::~StageRyu()
{
	
}

StageRyu::StageRyu(StageRyu&& _other) noexcept
{
}

//member Func
void StageRyu::Render()
{
	foreSpriteRender_->StartRender();
}
void StageRyu::LastCheck()
{
	foreSpriteRender_ = CreateSpriteRenderer("StageRyuFore.bmp");
}
void StageRyu::UpdateBefore()
{

}
void StageRyu::Update()
{
	
}
void StageRyu::UpdateAfter()
{

}

