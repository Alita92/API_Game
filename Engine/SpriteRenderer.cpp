#include "SpriteRenderer.h"
#include <EngineGeneralDebug.h>
#include "AssetManager.h"
#include <EngineGeneralTime.h>
#include "ImageAsset.h"
#include "Object.h"


SpriteRenderer::SpriteAnimation::SpriteAnimation()
	: startFrame_(-1),
	endFrame_(-1),
	curFrame_(-1),
	curInterTime_(-1),
	pivotEnum_(PIVOTENUM::BOTTOM)
	, linkedRenderer_(nullptr),
	linkedImage_(nullptr)
	, isLoop_(false),
	isOver_(false)
{

}
SpriteRenderer::SpriteRenderer()
	:imagePivot_(float4::ZERO),
	pivotpos_(float4::ZERO),
	rendersize_(float4::ZERO),
	imagepos_(float4::ZERO),
	imagesize_(float4::ZERO),
	image_(nullptr),
	curAnimation_(nullptr),
	object_(nullptr),
	isCameraLinked_(true),
	alpha_(255)
{
}

SpriteRenderer::~SpriteRenderer()
{
	std::map<std::string, SpriteAnimation*>::iterator AniStart = mapAnimation_.begin();
	std::map<std::string, SpriteAnimation*>::iterator AniEnd = mapAnimation_.end();

	for (; AniStart != AniEnd; ++AniStart)
	{
		if (nullptr == AniStart->second)
		{
			continue;
		}

		delete AniStart->second;
		AniStart->second = nullptr;
	}

	mapAnimation_.clear();

	
}

SpriteRenderer::SpriteRenderer(SpriteRenderer&& _other) noexcept
	:imagePivot_(float4::ZERO),
	pivotpos_(float4::ZERO),
	rendersize_(float4::ZERO),
	imagepos_(float4::ZERO),
	imagesize_(float4::ZERO),
	image_(nullptr),
	curAnimation_(nullptr),
	object_(nullptr),
	isCameraLinked_(true), alpha_(255)
{
}

//member Func
void SpriteRenderer::SetImageAsset(const std::string& _Name)
{
	image_ = AssetManager::GetAssetManager().FindImportedImage(_Name);


	if (nullptr == image_)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}
	// 아무 것도 넣어주지 않았다면 
	imagepos_ = float4::ZERO;
	imagesize_ = image_->GetImageSize();
	rendersize_ = image_->GetImageSize(); // 랜더링 사이즈는 고대로 이미지의 사이즈로...
	imagePivot_ = rendersize_.halffloat4(); // 이미지의 피벗 기본값 : 중심
}


void SpriteRenderer::StartRender()
{
	ImageAsset* screenImage = AssetManager::GetAssetManager().GetNextBuffer();

	float4 cameraLinkedObjectPos = object_->GetCameraLinkedPos();

	if (false == isCameraLinked_) // 만약 카메라의 위치를 따라가는 오브젝트가 아니라면...
	{
		cameraLinkedObjectPos = object_->GetPos();
	}

	float4 RenderPos = cameraLinkedObjectPos + pivotpos_ - imagePivot_;

	if (alpha_ == 255)
	{
		screenImage->MyTransBitCopy(image_,
			RenderPos,
			rendersize_,
			imagepos_,
			imagesize_,
			RGB(255, 0, 255));
	}
	else
	{
		screenImage->MyAlphaBitCopy(image_,
			alpha_,
			RenderPos,
			rendersize_,
			imagepos_,
			imagesize_);
	}
}


void SpriteRenderer::SetSpriteIndex(size_t _Index, PIVOTENUM _pivotStyle) // 잘라져서 가공된 스프라이트를 정리해준다
{
	if (false == image_->isCut())
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	imagepos_ = image_->GetCutLeftTop(_Index);
	imagesize_ = image_->GetCutRightBottom(_Index);
	rendersize_ = imagesize_;

	
	imagePivot_ = rendersize_.halffloat4(); // 스프라이트의 피벗을 중앙? 으로?

	switch (_pivotStyle)
	{
	case PIVOTENUM::MIDDLE: // 배경과 오브젝트 스프라이트는 Middle 피벗을 사용합니다.
		imagePivot_ = rendersize_.halffloat4();
		break;
	case PIVOTENUM::BOTTOM: // 캐릭터 스프라이트는 Bottom 피벗을 사용합니다.
		imagePivot_ = rendersize_.halffloat4();
		imagePivot_.y += rendersize_.halffloat4().y;
		break;
	case PIVOTENUM::LEFTTOP:
		imagePivot_ = float4::ZERO;
		break;

	default:
		EngineGeneralDebug::GameDestroyer();
		break;
	}
}



void SpriteRenderer::SpriteAnimation::UpdateAnimation()
{
	isOver_ = false;
	linkedRenderer_->SetSpriteIndex(curFrame_, pivotEnum_);
	curInterTime_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat(); // 델타타임을 계속 차감시킨다.

	if (0.0f >= curInterTime_) // 목표하는 지연 시간을 채웠다면...
	{
		++curFrame_;

		// curInterTime_ = interTime_; // 리셋
	

		if (endFrame_ < curFrame_)
		{
			isOver_ = true;

			if (true == isLoop_)
			{
				curFrame_ = startFrame_;
			}
			else
			{
				--curFrame_;
			}
		}
		curInterTime_ = interTime_[static_cast<size_t>(curFrame_ - startFrame_)];
	}
	linkedRenderer_->StartRender();
}


SpriteRenderer::SpriteAnimation* SpriteRenderer::FindSpriteAnimation(const std::string& _name)
{
	std::map<std::string, SpriteAnimation*>::iterator findIter = mapAnimation_.find(_name);

	if (findIter == mapAnimation_.end())
	{
		return nullptr;
	}

	return findIter->second;
}


void SpriteRenderer::CreateSpriteAnimation(const std::string& _name, const std::string& _imageName, int _startFrameOrder, int _endFrameOrder, bool _isLoop, float _delayTime,
	PIVOTENUM _pivotEnum)
{
	if (nullptr != FindSpriteAnimation(_name))
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	ImageAsset* imageAssetPtr = AssetManager::GetAssetManager().FindImportedImage(_imageName); // 임포트된 이미지가 없다?

	if (nullptr == imageAssetPtr)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	if (false == imageAssetPtr->isCut()) // 잘려있지 않다면 파괴
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	SpriteAnimation* newAnimation = new SpriteAnimation();

	newAnimation->SetName(_name);
	newAnimation->startFrame_ = _startFrameOrder; // ~ 번째부터의 스프라이트부터
	newAnimation->endFrame_ = _endFrameOrder; // ~ 번째까지의 스프라이트를 사용할지


	newAnimation->interTime_.reserve(static_cast<size_t>(newAnimation->endFrame_) - static_cast<size_t>(newAnimation->startFrame_)); // Reserve 도 +1 되어야 하지 않을까?
	for (size_t i = 0; i < (static_cast<size_t>(newAnimation->endFrame_) - static_cast<size_t>(newAnimation->startFrame_)) + 1; i++)
	{
		newAnimation->interTime_.push_back(_delayTime);
	} // 동적 할당으로 들어온 프레임을 하나 하나 나눠 프레임마다 저장해준다.
	newAnimation->linkedImage_ = imageAssetPtr;
	newAnimation->curFrame_ = _startFrameOrder;
	newAnimation->curInterTime_ = _delayTime;
	newAnimation->pivotEnum_ = _pivotEnum;
	newAnimation->linkedRenderer_ = this;
	newAnimation->isLoop_ = _isLoop;

	mapAnimation_.insert(std::map<std::string, SpriteAnimation*>::value_type(_name, newAnimation));
	// 맵구조 등록
}

void SpriteRenderer::ChangeSpriteAnimation(const std::string& _name, bool _isForced)
{
	// 기존 애니메이션이 진행중이거나
	if (nullptr != curAnimation_ &&
		// 강제바꾸기가 아니고
		false == _isForced &&
		// 애니메이션 이름이 같으면
		curAnimation_->GetName() == _name)
	{
		return; // 암것도 안 해주고 함수를 끝낸다.
	}

	curAnimation_ = FindSpriteAnimation(_name); // 찾은 다음 애니메이션을 찾고...
	curAnimation_->Reboot(); // 내부 값을 리셋해준다.

	if (nullptr == curAnimation_)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}
}

void SpriteRenderer::FinalAnimationUpdate()
{
	if (nullptr == curAnimation_)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	curAnimation_->UpdateAnimation();
}





bool SpriteRenderer::IsCurSpriteAnimationName(const std::string& _name)
{
	if (nullptr == curAnimation_)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	if (curAnimation_->GetName() == _name)
	{
		return true;
	}

	return false;
}

bool SpriteRenderer::IsCurSpriteAnimationEnd()
{
	if (nullptr == curAnimation_)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	return curAnimation_->isOver_;
}

void SpriteRenderer::SetChangeFrame(const std::string& _Name, int _index, float _Time)
{
	SpriteAnimation* FindAni = FindSpriteAnimation(_Name);

	if (nullptr == FindAni)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	FindAni->interTime_[(static_cast<size_t>(_index) - static_cast<size_t>(FindAni->startFrame_))] = _Time;
	// 문제 : interTime_[0], interTime_[1] 을 1번째, 2번째 프레임이 공유 중이다.
}

int SpriteRenderer::GetCurAnimationFrame()
{
	if (nullptr == curAnimation_)
	{
		EngineGeneralDebug::GameDestroyer();
		return 0;
	}

	return curAnimation_->curFrame_;
}