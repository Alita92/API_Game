#include "TimeCounter.h"
#include <SpriteRenderer.h>
#include <EngineGeneralMath.h>
#include "FightScene.h"
#include <EngineGeneralTime.h>

// Static Var
// Static Func

// constructer destructer
TimeCounter::TimeCounter()
	: mainSpriteRenderer_(nullptr), roundTime_(99), unitTime_(1.0f)
{
	SetRenderOrder(30);
}

TimeCounter::~TimeCounter()
{
}

TimeCounter::TimeCounter(TimeCounter&& _other) noexcept
	: mainSpriteRenderer_(nullptr), roundTime_(99), unitTime_(1.0f)
{
}

//member Func
void TimeCounter::Render()
{
	mainSpriteRenderer_->FinalAnimationUpdate();
}

void TimeCounter::spriteInit()
{
	mainSpriteRenderer_ = CreateSpriteRenderer("Numbers.bmp");

	mainSpriteRenderer_->CreateSpriteAnimation("1", "Numbers.bmp", 0, 0, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("2", "Numbers.bmp", 1, 1, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("3", "Numbers.bmp", 2, 2, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("4", "Numbers.bmp", 3, 3, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("5", "Numbers.bmp", 4, 4, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("6", "Numbers.bmp", 5, 5, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("7", "Numbers.bmp", 6, 6, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("8", "Numbers.bmp", 7, 7, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("9", "Numbers.bmp", 8, 8, false, 1.0f);
	mainSpriteRenderer_->CreateSpriteAnimation("0", "Numbers.bmp", 9, 9, false, 1.0f);
}

void TimeCounter::timeInit()
{
	if ("Tendigits" == GetName())
	{
		digitsTag_ = "Tendigits";

		friendDigits_ = DIGITSTYLE::ONEDIGIT;
	}
	if ("Onedigits" == GetName())
	{
		digitsTag_ = "Onedigits";

		friendDigits_ = DIGITSTYLE::TENDIGIT;
	}
}


void TimeCounter::LastCheck()
{
	spriteInit();
	//tenDigitSpriteRenderer_->SetImagePos({ 554,120 });
	//oneDigitSpriteRenderer_->SetImagePos({ 597, 120 });
	//tenDigitSpriteRenderer_->ChangeSpriteAnimation("10");
	mainSpriteRenderer_->ChangeSpriteAnimation("9");
	mainSpriteRenderer_->SetCameraEffectOff();

}
void TimeCounter::UpdateBefore()
{

}
void TimeCounter::Update()
{
	if (true == IsOn())
	{
		unitTime_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat();
		if (unitTime_ <= 0.0f)
		{
			roundTime_--;
			if (0 == roundTime_)
			{
				this->Off();
				return;
			}

			if ("Tendigits" == GetName())
			{

				int roundTimeTenDigit = roundTime_ / 10;
				switch (roundTimeTenDigit)
				{
				case 0:
					mainSpriteRenderer_->ChangeSpriteAnimation("0");

					break;
				case 1:
					mainSpriteRenderer_->ChangeSpriteAnimation("1");

					break;
				case 2:
					mainSpriteRenderer_->ChangeSpriteAnimation("2");

					break;
				case 3:
					mainSpriteRenderer_->ChangeSpriteAnimation("3");

					break;
				case 4:
					mainSpriteRenderer_->ChangeSpriteAnimation("4");
					break;
				case 5:
					mainSpriteRenderer_->ChangeSpriteAnimation("5");

					break;
				case 6:
					mainSpriteRenderer_->ChangeSpriteAnimation("6");

					break;
				case 7:
					mainSpriteRenderer_->ChangeSpriteAnimation("7");

					break;
				case 8:
					mainSpriteRenderer_->ChangeSpriteAnimation("8");

					break;
				case 9:
					mainSpriteRenderer_->ChangeSpriteAnimation("9");

					break;
				default:
					break;
				}
				unitTime_ = 1.0f;
			}

			if ("Onedigits" == GetName())
			{

				int roundTimeOneDigit = (roundTime_ % 10);
				switch (roundTimeOneDigit)
				{
				case 0:
					mainSpriteRenderer_->ChangeSpriteAnimation("0");
					break;
				case 1:
					mainSpriteRenderer_->ChangeSpriteAnimation("1");
					break;
				case 2:
					mainSpriteRenderer_->ChangeSpriteAnimation("2");
					break;
				case 3:
					mainSpriteRenderer_->ChangeSpriteAnimation("3");
					break;
				case 4:
					mainSpriteRenderer_->ChangeSpriteAnimation("4");
					break;
				case 5:
					mainSpriteRenderer_->ChangeSpriteAnimation("5");
					break;
				case 6:
					mainSpriteRenderer_->ChangeSpriteAnimation("6");
					break;
				case 7:
					mainSpriteRenderer_->ChangeSpriteAnimation("7");
					break;
				case 8:
					mainSpriteRenderer_->ChangeSpriteAnimation("8");
					break;
				case 9:
					mainSpriteRenderer_->ChangeSpriteAnimation("9");
					break;
				default:
					break;
				}
				unitTime_ = 1.0f;
				return;
			}
		}
		return;
	}
	return;
}




void TimeCounter::UpdateAfter()
{

}