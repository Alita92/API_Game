#include "SelectionPortrait.h"
#include <SpriteRenderer.h>
#include "Cursor.h"
// Static Var
// Static Func

// constructer destructer
SelectionPortrait::SelectionPortrait()
{
	SetRenderOrder(10);
}

SelectionPortrait::~SelectionPortrait()
{
}

SelectionPortrait::SelectionPortrait(SelectionPortrait&& _other) noexcept
{
}

//member Func

void SelectionPortrait::LastCheck()
{

	AnimationInit();
	
}

void SelectionPortrait::AnimationInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("CharacterPortraitSmall.bmp");
	mainSpriteRender_->CreateSpriteAnimation("P1Ryu", "CharacterPortraitSmall.bmp", 0, 0, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Ehonda", "CharacterPortraitSmall.bmp", 1, 1, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Blanka", "CharacterPortraitSmall.bmp", 2, 2, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Guile", "CharacterPortraitSmall.bmp", 3, 3, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Ken", "CharacterPortraitSmall.bmp", 4, 4, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1ChunLi", "CharacterPortraitSmall.bmp", 5, 5, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Zangief", "CharacterPortraitSmall.bmp", 6, 6, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Dhalsim", "CharacterPortraitSmall.bmp", 7, 7, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1THawk", "CharacterPortraitSmall.bmp", 8, 8, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1FeiLong", "CharacterPortraitSmall.bmp", 9, 9, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1DeeJay", "CharacterPortraitSmall.bmp", 10, 10, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Cammy", "CharacterPortraitSmall.bmp", 11, 11, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Boxer", "CharacterPortraitSmall.bmp", 12, 12, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Claw", "CharacterPortraitSmall.bmp", 13, 13, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Sagat", "CharacterPortraitSmall.bmp", 14, 14, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P1Dictator", "CharacterPortraitSmall.bmp", 15, 15, false, 0.102f);

	mainSpriteRender_->CreateSpriteAnimation("P2Ryu", "CharacterPortraitSmall.bmp",	20, 20, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Ehonda", "CharacterPortraitSmall.bmp", 21, 21, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Blanka", "CharacterPortraitSmall.bmp", 22, 22, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Guile", "CharacterPortraitSmall.bmp", 23, 23, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Ken", "CharacterPortraitSmall.bmp", 24, 24, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2ChunLi", "CharacterPortraitSmall.bmp", 25, 25, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Zangief", "CharacterPortraitSmall.bmp", 26, 26, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Dhalsim", "CharacterPortraitSmall.bmp", 27, 27, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2THawk", "CharacterPortraitSmall.bmp", 28, 28, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2FeiLong", "CharacterPortraitSmall.bmp", 29, 29, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2DeeJay", "CharacterPortraitSmall.bmp", 30, 30, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Cammy", "CharacterPortraitSmall.bmp", 31, 31, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Boxer", "CharacterPortraitSmall.bmp", 32, 32, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Claw", "CharacterPortraitSmall.bmp", 33, 33, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Sagat", "CharacterPortraitSmall.bmp", 34, 34, false, 0.102f);
	mainSpriteRender_->CreateSpriteAnimation("P2Dictator", "CharacterPortraitSmall.bmp", 35, 35, false, 0.102f);
	mainSpriteRender_->ChangeSpriteAnimation("P1Ryu");
}

void SelectionPortrait::PlayerInit()
{
	if ("p1Portrait" == GetName())
	{
		ChangePortrait("P1Ryu");
	}

	if ("p2Portrait" == GetName())
	{
		ChangePortrait("P2Ken");
	}
}

void SelectionPortrait::UpdateBefore()
{

}
void SelectionPortrait::Update()
{
	// 커서 위치에 따라 애니메이션을 변경해주기
	// parentCursor_->ReturnLocation(); // 이 값을 이용해서
}
void SelectionPortrait::UpdateAfter()
{

}
void SelectionPortrait::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}

void SelectionPortrait::ChangePortrait(std::string _name)
{
	mainSpriteRender_->ChangeSpriteAnimation(_name);
}