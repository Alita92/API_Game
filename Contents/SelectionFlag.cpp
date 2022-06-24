#include "SelectionFlag.h"
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
SelectionFlag::SelectionFlag()
	:mainSpriteRender_(nullptr)
{
	SetRenderOrder(10);
}

SelectionFlag::~SelectionFlag()
{
}

SelectionFlag::SelectionFlag(SelectionFlag&& _other) noexcept
	:mainSpriteRender_(nullptr)
{
}

//member Func
void SelectionFlag::LastCheck()
{
	AnimationInit();
}

void SelectionFlag::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("Flag.bmp");
	mainSpriteRender_->CreateSpriteAnimation("Japan", "Flag.bmp", 0, 0, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Brazil", "Flag.bmp", 1, 1, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("India", "Flag.bmp", 2, 2, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Mexico", "Flag.bmp", 3, 3, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("USA", "Flag.bmp", 4, 4, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Thailand", "Flag.bmp", 5, 5, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("China", "Flag.bmp", 6, 6, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("USSR", "Flag.bmp", 7, 7, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Spain", "Flag.bmp", 8, 8, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("England", "Flag.bmp", 9, 9, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Jamaica", "Flag.bmp", 10, 10, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("Hongkong", "Flag.bmp", 11, 11, false, 0.1f);
}

void SelectionFlag::PlayerInit()
{
	if ("p1Flag" == GetName())
	{
		SetPos({1056,204}); 
		mainSpriteRender_->ChangeSpriteAnimation("Japan");
	}

	if ("p2Flag" == GetName())
	{
		SetPos({432, 131});
		mainSpriteRender_->ChangeSpriteAnimation("USA");
	}
}

void SelectionFlag::UpdateBefore()
{

}

void SelectionFlag::Update()
{

}
void SelectionFlag::UpdateAfter()
{

}
void SelectionFlag::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}

void SelectionFlag::ChangeFlagAndPos(std::string _flag, float4 _pos)
{
	mainSpriteRender_->ChangeSpriteAnimation(_flag);
	SetPos(_pos);
}