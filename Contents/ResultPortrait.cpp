#include "ResultPortrait.h"
#include "CharacterSelectScene.h"
#include "FightScene.h"
#include <EngineScreen.h>
#include <ImageAsset.h>
#include <AssetManager.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <EngineGeneralTime.h>
#include <SpriteRenderer.h>

// Static Var
// Static Func

// constructer destructer
ResultPortrait::ResultPortrait()
	
{
	SetRenderOrder(30);
}

ResultPortrait::~ResultPortrait()
{
}

ResultPortrait::ResultPortrait(ResultPortrait&& _other) noexcept
{
}

//member Func

void ResultPortrait::portraitInit()
{
	mainSpriteRender_ = CreateSpriteRenderer("Portrait.bmp");
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitClaw", "Portrait.bmp", 0, 0, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("P1PortraitBlanka", "Portrait.bmp", 1, 1, false, 0.1f);
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

	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitClaw", "Portrait.bmp", 40, 40, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitBlanka", "Portrait.bmp", 41, 41, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitCammy", "Portrait.bmp", 42, 42, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitChunLi", "Portrait.bmp", 43, 43, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitDeeJay", "Portrait.bmp", 44, 44, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitDhalsim", "Portrait.bmp", 45, 45, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitEHonda", "Portrait.bmp", 46, 46, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitFeiLong", "Portrait.bmp", 47, 47, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitGuile", "Portrait.bmp", 48, 48, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitKen", "Portrait.bmp", 49, 49, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitBoxer", "Portrait.bmp", 50, 50, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitRyu", "Portrait.bmp", 51, 51, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitSagat", "Portrait.bmp", 52, 52, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitTHawk", "Portrait.bmp", 53, 53, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitDictator", "Portrait.bmp", 54, 54, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP2PortraitZangief", "Portrait.bmp", 55, 55, false, 0.1f);

	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitClaw", "Portrait.bmp", 60, 60, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitBlanka", "Portrait.bmp", 61, 61, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitCammy", "Portrait.bmp", 62, 62, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitChunLi", "Portrait.bmp", 63, 63, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitDeeJay", "Portrait.bmp", 64, 64, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitDhalsim", "Portrait.bmp", 65, 65, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitEHonda", "Portrait.bmp", 66, 66, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitFeiLong", "Portrait.bmp", 67, 67, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitGuile", "Portrait.bmp", 68, 68, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitKen", "Portrait.bmp", 69, 69, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitBoxer", "Portrait.bmp", 70, 70, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitRyu", "Portrait.bmp", 71, 71, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitSagat", "Portrait.bmp", 72, 72, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitTHawk", "Portrait.bmp", 73, 73, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitDictator", "Portrait.bmp", 74, 74, false, 0.1f);
	mainSpriteRender_->CreateSpriteAnimation("LP1PortraitZangief", "Portrait.bmp", 75, 75, false, 0.1f);

	mainSpriteRender_->ChangeSpriteAnimation("P1PortraitRyu");


}
void ResultPortrait::LastCheck()
{
	portraitInit();
}

void ResultPortrait::UpdateBefore()
{

}
void ResultPortrait::Update()
{
	if ("P1" == GetName())
	{
		if ("" == CharacterSelectScene::p1Selection_)
		{
			EngineGeneralDebug::MsgBoxError("P1에 선택된 캐릭터 없이 Result Scene 에 왔습니다.");
			return;
		}
		if ("P1" == FightScene::whoWon_)
		{
			mainSpriteRender_->ChangeSpriteAnimation("P1Portrait" + CharacterSelectScene::p1Selection_, false);
			return;
		}
		else
		{
			mainSpriteRender_->ChangeSpriteAnimation("LP1Portrait" + CharacterSelectScene::p1Selection_, false);
			return;
		}
	}
	if ("P2" == GetName())
	{
		if ("" == CharacterSelectScene::p2Selection_)
		{
			EngineGeneralDebug::MsgBoxError("P2에 선택된 캐릭터 없이 Result Scene 에 왔습니다.");
			return;
		}
		if ("P2" == FightScene::whoWon_)
		{
			mainSpriteRender_->ChangeSpriteAnimation("P2Portrait" + CharacterSelectScene::p2Selection_, false);
			return;
		}
		else
		{
			mainSpriteRender_->ChangeSpriteAnimation("LP2Portrait" + CharacterSelectScene::p2Selection_, false);
			return;
		}
	}
}
void ResultPortrait::UpdateAfter()
{

}
void ResultPortrait::Render()
{
	mainSpriteRender_->FinalAnimationUpdate();
}
void ResultPortrait::Collision()
{

}