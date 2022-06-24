#include "ContinueKOCounter.h"
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
ContinueKOCounter::ContinueKOCounter()
{
	SetRenderOrder(21);
	SetPos({ 776, 336 });
}

ContinueKOCounter::~ContinueKOCounter()
{
}

ContinueKOCounter::ContinueKOCounter(ContinueKOCounter&& _other) noexcept
{
}

//member Func
void ContinueKOCounter::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("ContinueCounter.bmp");
	mainSpriteRender_->CreateSpriteAnimation("9", "ContinueCounter.bmp", 0, 0, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("8", "ContinueCounter.bmp", 1, 1, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("7", "ContinueCounter.bmp", 2, 2, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("6", "ContinueCounter.bmp", 3, 3, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("5", "ContinueCounter.bmp", 4, 4, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("4", "ContinueCounter.bmp", 5, 5, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("3", "ContinueCounter.bmp", 6, 6, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("2", "ContinueCounter.bmp", 7, 7, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("1", "ContinueCounter.bmp", 8, 8, false, 0.017f);
	mainSpriteRender_->CreateSpriteAnimation("0", "ContinueCounter.bmp", 9, 9, false, 0.017f);
	mainSpriteRender_->ChangeSpriteAnimation("9");
}

void ContinueKOCounter::LastCheck()
{
	AnimationInit();
}

void ContinueKOCounter::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}

void ContinueKOCounter::Update()
{

}

void ContinueKOCounter::ChangeNumber(std::string _number)
{
	mainSpriteRender_->ChangeSpriteAnimation(_number);
}