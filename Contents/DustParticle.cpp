#include "DustParticle.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>

// Static Var
// Static Func

// constructer destructer
DustParticle::DustParticle()
{
	SetRenderOrder(11);
}

DustParticle::~DustParticle()
{
}

DustParticle::DustParticle(DustParticle&& _other) noexcept
{
}

//member Func
void DustParticle::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("FightingEffects.bmp");
	mainSpriteRender_->CreateSpriteAnimation("Dust", "FightingEffects.bmp", 7, 9, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("Void", "FightingEffects.bmp", 10, 10, false, 0.102f);
	mainSpriteRender_->ChangeSpriteAnimation("Dust");
	mainSpriteRender_->SetCameraEffectOn();
}

void DustParticle::Update()
{
	SetMove(float4::UP * EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat() * 50.0f);
}

void DustParticle::Render()
{
		mainSpriteRender_->FinalAnimationUpdate();

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			mainSpriteRender_->ChangeSpriteAnimation("Void");
			Death();
		}
}