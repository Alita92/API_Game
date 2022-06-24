#include "VSPortrait.h"
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
VSPortrait::VSPortrait()
{
	SetRenderOrder(52);
}

VSPortrait::~VSPortrait()
{
}

VSPortrait::VSPortrait(VSPortrait&& _other) noexcept
{
}

//member Func
void VSPortrait::Render()
{
	if (true == IsOn())
	{
		mainSpriteRender_->FinalAnimationUpdate();
	}
}
void VSPortrait::LastCheck()
{
	AnimationInit();
}

void VSPortrait::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("Portrait.bmp");
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitClaw","Portrait.bmp", 0, 0, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitBlanka","Portrait.bmp", 1, 1, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitCammy", "Portrait.bmp", 2, 2, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitChunLi", "Portrait.bmp", 3, 3, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitDeeJay", "Portrait.bmp", 4, 4, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitDhalsim", "Portrait.bmp", 5, 5, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitEHonda", "Portrait.bmp", 6, 6, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitFeiLong", "Portrait.bmp", 7, 7, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitGuile", "Portrait.bmp", 8, 8, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitKen", "Portrait.bmp", 9, 9, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitBoxer", "Portrait.bmp", 10, 10, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitRyu", "Portrait.bmp", 11, 11, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitSagat", "Portrait.bmp", 12, 12, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitTHawk", "Portrait.bmp", 13, 13, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitDictator", "Portrait.bmp", 14, 14, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitZangief", "Portrait.bmp", 15, 15, false, 0.1f);

	mainSpriteRender_->CreateSpriteAnimation("P2PortraitClaw", "Portrait.bmp", 20, 20, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitBlanka", "Portrait.bmp", 21, 21, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitCammy", "Portrait.bmp", 22, 22, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitChunLi", "Portrait.bmp", 23, 23, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitDeeJay", "Portrait.bmp", 24, 24, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitDhalsim", "Portrait.bmp", 25, 25, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitEHonda", "Portrait.bmp", 26, 26, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitFeiLong", "Portrait.bmp", 27, 27, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitGuile", "Portrait.bmp", 28, 28, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitKen", "Portrait.bmp", 29, 29, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitBoxer", "Portrait.bmp", 30, 30, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitRyu", "Portrait.bmp", 31, 31, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitSagat", "Portrait.bmp", 32, 32, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitTHawk", "Portrait.bmp", 33, 33, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitDictator", "Portrait.bmp", 34, 34, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P2PortraitZangief", "Portrait.bmp", 35, 35, false, 0.1f);
}

void VSPortrait::SetPortrait(std::string _name)
{
	mainSpriteRender_->ChangeSpriteAnimation(_name);
}