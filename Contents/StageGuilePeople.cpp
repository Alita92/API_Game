#include "StageGuilePeople.h"
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
StageGuilePeople::StageGuilePeople()
{
	SetRenderOrder(6);
}

StageGuilePeople::~StageGuilePeople()
{
}

StageGuilePeople::StageGuilePeople(StageGuilePeople&& _other) noexcept
{
}

//member Func
void StageGuilePeople::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}

void StageGuilePeople::LastCheck()
{
	AnimationInIt();
	mainSpriteRender_->SetCameraEffectOn();
}

void StageGuilePeople::Update()
{

}

void StageGuilePeople::AnimationInIt()
{
	mainSpriteRender_ = CreateSpriteRenderer("StageGuilePeople.bmp");
	mainSpriteRender_->CreateSpriteAnimation("People1", "StageGuilePeople.bmp", 0, 1, true, 0.306f);
	mainSpriteRender_->CreateSpriteAnimation("People2", "StageGuilePeople.bmp", 2, 3, true, 0.266f);
	mainSpriteRender_->CreateSpriteAnimation("People3", "StageGuilePeople.bmp", 4, 5, true, 0.354f);
	mainSpriteRender_->ChangeSpriteAnimation("People1");
}

void StageGuilePeople::ChangeAnimation(std::string _name)
{
	mainSpriteRender_->ChangeSpriteAnimation(_name);
}