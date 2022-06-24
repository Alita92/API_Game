#include "HitPointName.h"
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
HitPointName::HitPointName()
{
	SetRenderOrder(20);
}

HitPointName::~HitPointName()
{
}

HitPointName::HitPointName(HitPointName&& _other) noexcept
{
}

//member Func
void HitPointName::Render()
{
		mainSpriteRender_->FinalAnimationUpdate();
}
void HitPointName::LastCheck()
{
	AnimationInit();
	mainSpriteRender_->SetCameraEffectOff();
}

void HitPointName::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("FightName.bmp");
	mainSpriteRender_->CreateSpriteAnimation("Ken", "VSName.bmp", 0, 0, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Ryu", "VSName.bmp", 1, 1, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Guile", "VSName.bmp", 2, 2, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("ChunLi", "VSName.bmp", 3, 3, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Sagat", "VSName.bmp", 4, 4, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Cammy", "VSName.bmp", 5, 5, false, 0.1f);

}

void HitPointName::SetHitPointName(std::string _name)
{
	mainSpriteRender_->ChangeSpriteAnimation(_name);
}