#include "HitPointGauge.h"
#include <EngineScreen.h>
#include <EngineGeneralTime.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <SpriteRenderer.h>
#include <EngineGeneralMath.h>
#include <Scene.h>
#include "FightScene.h"
#include "CharacterHandle.h"


// Static Var
// Static Func

// constructer destructer
HitPointGauge::HitPointGauge()
{
	SetPos(EngineScreen::GetScreen().GetSize().halffloat4());
	SetRenderOrder(20);

}

HitPointGauge::~HitPointGauge()
{
}

HitPointGauge::HitPointGauge(HitPointGauge&& _other) noexcept
{
}

void HitPointGauge::Render()
{
	mainSpriteRender_->StartRender();

}
void HitPointGauge::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("HitPointGauge.bmp");
	mainSpriteRender_->SetCameraEffectOff();

}
void HitPointGauge::UpdateBefore()
{

}
void HitPointGauge::Update()
{

	
}
void HitPointGauge::UpdateAfter()
{

}

void HitPointGauge::Collision()
{

}