#include "VSName.h"
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
VSName::VSName()
{
	SetRenderOrder(52);
}

VSName::~VSName()
{
}

VSName::VSName(VSName&& _other) noexcept
{
}

//member Func
void VSName::Render()
{
	if (true == IsOn())
	{
		mainSpriteRender_->FinalAnimationUpdate();
	}
}
void VSName::LastCheck()
{
	AnimationInit();
}

void VSName::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("VSName.bmp");
	mainSpriteRender_->CreateSpriteAnimation("Ken", "VSName.bmp", 0, 0, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Ryu", "VSName.bmp", 1, 1, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Guile", "VSName.bmp", 2, 2, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("ChunLi", "VSName.bmp", 3, 3, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Sagat", "VSName.bmp", 4, 4, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Cammy", "VSName.bmp", 5, 5, false, 0.1f);

}

void VSName::SetVSName(std::string _name)
{
	mainSpriteRender_->ChangeSpriteAnimation(_name);
}