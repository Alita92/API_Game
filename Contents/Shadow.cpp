#include "Shadow.h"
#include <SpriteRenderer.h>
#include <EngineGeneralTime.h>
#include "CharacterHandle.h"

// Static Var
// Static Func

// constructer destructer
Shadow::Shadow()
{
	SetRenderOrder(9);
}

Shadow::~Shadow()
{
}

Shadow::Shadow(Shadow&& _other) noexcept
{
}

//member Func
void Shadow::PosInit()
{
	SetPos({ parentCharacter_->GetPos().x, 583.0f });
}

void Shadow::LastCheck()
{
	mainSpriteRender_ = CreateSpriteRenderer("Shadow.bmp");
}

void Shadow::Update()
{
	SetPos({ parentCharacter_->GetPos().x, 583.0f });
}

void Shadow::Render()
{
	mainSpriteRender_->StartRender();
}