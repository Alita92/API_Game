#include "FightEffects.h"
#include <SpriteRenderer.h>
#include "CharacterHandle.h"
#include "ContentsENUM.h"
// Static Var
// Static Func

// constructer destructer
FightEffects::FightEffects()
{
	SetRenderOrder(11);
}

FightEffects::~FightEffects()
{
}

FightEffects::FightEffects(FightEffects&& _other) noexcept
{
}

//member Func
void FightEffects::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("FightingEffects.bmp");
	mainSpriteRender_->CreateSpriteAnimation("HitEffect", "FightingEffects.bmp", 0, 3, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("GuardEffect", "FightingEffects.bmp", 4, 6, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("Dust", "FightingEffects.bmp", 7, 9, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("Void", "FightingEffects.bmp", 10, 10, false, 0.102f);
	mainSpriteRender_->ChangeSpriteAnimation("Void");
	mainSpriteRender_->SetCameraEffectOn();
}
void FightEffects::UpdateBefore()
{

}
void FightEffects::Update()
{
	if ("HIGH" == parentCharacter_->GetHitEffectLocation(parentCharacter_->curHitstyle_))
	{
		SetPos({ parentCharacter_->otherCharacter_->GetPos().x, parentCharacter_->otherCharacter_->GetPos().y - 48 }); // 상단
		return;
	}
	
	if ("MID" == parentCharacter_->GetHitEffectLocation(parentCharacter_->curHitstyle_))
	{
		SetPos({ parentCharacter_->otherCharacter_->GetPos().x  , parentCharacter_->otherCharacter_->GetPos().y }); // 중단
		return;
	}

	if ("LOW" == parentCharacter_->GetHitEffectLocation(parentCharacter_->curHitstyle_))
	{
		SetPos({ parentCharacter_->otherCharacter_->GetPos().x, parentCharacter_->otherCharacter_->GetPos().y + 48 }); // 하단
		return;
	}
}

void FightEffects::UpdateAfter()
{

}
void FightEffects::Render()
{
	if (true == IsOn())
	{
		mainSpriteRender_->FinalAnimationUpdate();

		if (true == mainSpriteRender_->IsCurSpriteAnimationEnd())
		{
			mainSpriteRender_->ChangeSpriteAnimation("Void");
			Off();
		}
	}
}
void FightEffects::Collision()
{

}