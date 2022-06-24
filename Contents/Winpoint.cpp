#include "Winpoint.h"
#include <SpriteRenderer.h>
#include "FightScene.h"

// Static Var
// Static Func

// constructer destructer
Winpoint::Winpoint()
{
	SetRenderOrder(20);
}

Winpoint::~Winpoint()
{
}

Winpoint::Winpoint(Winpoint&& _other) noexcept
{
}

//member Func
void Winpoint::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("Winpoint.bmp");
	mainSpriteRender_->SetCameraEffectOff();
}

void Winpoint::Render()
{
	if (true == IsOn())
	{
		mainSpriteRender_->StartRender();
	}
}

void Winpoint::Update()
{
	if ("P1" == GetName())
	{
		SetPos({50,80});
		if (FightScene::p1WinPoint_ >= 1)
		{
			On();
		}
	}

	if ("P2" == GetName())
	{
		SetPos({1102, 80});
		if (FightScene::p2WinPoint_ >= 1)
		{
			On();
		}
	}

	
}