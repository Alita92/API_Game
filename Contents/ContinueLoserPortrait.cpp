#include "ContinueLoserPortrait.h"
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
ContinueLoserPortrait::ContinueLoserPortrait()
{
	SetRenderOrder(21);
	SetPos({ 376,336 });
}

ContinueLoserPortrait::~ContinueLoserPortrait()
{
}

ContinueLoserPortrait::ContinueLoserPortrait(ContinueLoserPortrait&& _other) noexcept
{
}

//member Func
void ContinueLoserPortrait::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("Portrait.bmp");
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitClaw", "Portrait.bmp", 60, 60, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitBlanka", "Portrait.bmp", 61, 61, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitCammy", "Portrait.bmp", 62, 62, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitChunLi", "Portrait.bmp", 63, 63, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitDeeJay", "Portrait.bmp", 64, 64, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitDhalsim", "Portrait.bmp", 65, 65, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitEHonda", "Portrait.bmp", 66, 66, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitFeiLong", "Portrait.bmp", 67, 67, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitGuile", "Portrait.bmp", 68, 68, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitKen", "Portrait.bmp", 69, 69, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitBoxer", "Portrait.bmp", 70, 70, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitRyu", "Portrait.bmp", 71, 71, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitSagat", "Portrait.bmp", 72, 72, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitTHawk", "Portrait.bmp", 73, 73, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitDictator", "Portrait.bmp", 74, 74, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitZangief", "Portrait.bmp", 75, 75, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LoserPortraitDefault", "Portrait.bmp", 76, 76, false, 0.1f);
	mainSpriteRender_->ChangeSpriteAnimation("LoserPortraitDefault");
}

void ContinueLoserPortrait::LastCheck()
{
	AnimationInit();
}

void ContinueLoserPortrait::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}

void ContinueLoserPortrait::Update()
{

}

void ContinueLoserPortrait::ChangePortrait(std::string _name)
{
	mainSpriteRender_->ChangeSpriteAnimation("LoserPortrait" + _name);
}
