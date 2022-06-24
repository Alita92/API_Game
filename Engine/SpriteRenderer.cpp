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
	// �ƹ� �͵� �־����� �ʾҴٸ� 
	imagepos_ = float4::ZERO;
	imagesize_ = image_->GetImageSize();
	rendersize_ = image_->GetImageSize(); // ������ ������� ���� �̹����� �������...
	imagePivot_ = rendersize_.halffloat4(); // �̹����� �ǹ� �⺻�� : �߽�
}


void SpriteRenderer::StartRender()
{
	ImageAsset* screenImage = AssetManager::GetAssetManager().GetNextBuffer();

	float4 cameraLinkedObjectPos = object_->GetCameraLinkedPos();

	if (false == isCameraLinked_) // ���� ī�޶��� ��ġ�� ���󰡴� ������Ʈ�� �ƴ϶��...
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


void SpriteRenderer::SetSpriteIndex(size_t _Index, PIVOTENUM _pivotStyle) // �߶����� ������ ��������Ʈ�� �������ش�
{
	if (false == image_->isCut())
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	imagepos_ = image_->GetCutLeftTop(_Index);
	imagesize_ = image_->GetCutRightBottom(_Index);
	rendersize_ = imagesize_;

	
	imagePivot_ = rendersize_.halffloat4(); // ��������Ʈ�� �ǹ��� �߾�? ����?

	switch (_pivotStyle)
	{
	case PIVOTENUM::MIDDLE: // ���� ������Ʈ ��������Ʈ�� Middle �ǹ��� ����մϴ�.
		imagePivot_ = rendersize_.halffloat4();
		break;
	case PIVOTENUM::BOTTOM: // ĳ���� ��������Ʈ�� Bottom �ǹ��� ����մϴ�.
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
	curInterTime_ -= EngineGeneralTime::GetEngineGeneralTime().GetDeltaTimeFloat(); // ��ŸŸ���� ��� ������Ų��.

	if (0.0f >= curInterTime_) // ��ǥ�ϴ� ���� �ð��� ä���ٸ�...
	{
		++curFrame_;

		// curInterTime_ = interTime_; // ����
	

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

	ImageAsset* imageAssetPtr = AssetManager::GetAssetManager().FindImportedImage(_imageName); // ����Ʈ�� �̹����� ����?

	if (nullptr == imageAssetPtr)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	if (false == imageAssetPtr->isCut()) // �߷����� �ʴٸ� �ı�
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	SpriteAnimation* newAnimation = new SpriteAnimation();

	newAnimation->SetName(_name);
	newAnimation->startFrame_ = _startFrameOrder; // ~ ��°������ ��������Ʈ����
	newAnimation->endFrame_ = _endFrameOrder; // ~ ��°������ ��������Ʈ�� �������


	newAnimation->interTime_.reserve(static_cast<size_t>(newAnimation->endFrame_) - static_cast<size_t>(newAnimation->startFrame_)); // Reserve �� +1 �Ǿ�� ���� ������?
	for (size_t i = 0; i < (static_cast<size_t>(newAnimation->endFrame_) - static_cast<size_t>(newAnimation->startFrame_)) + 1; i++)
	{
		newAnimation->interTime_.push_back(_delayTime);
	} // ���� �Ҵ����� ���� �������� �ϳ� �ϳ� ���� �����Ӹ��� �������ش�.
	newAnimation->linkedImage_ = imageAssetPtr;
	newAnimation->curFrame_ = _startFrameOrder;
	newAnimation->curInterTime_ = _delayTime;
	newAnimation->pivotEnum_ = _pivotEnum;
	newAnimation->linkedRenderer_ = this;
	newAnimation->isLoop_ = _isLoop;

	mapAnimation_.insert(std::map<std::string, SpriteAnimation*>::value_type(_name, newAnimation));
	// �ʱ��� ���
}

void SpriteRenderer::ChangeSpriteAnimation(const std::string& _name, bool _isForced)
{
	// ���� �ִϸ��̼��� �������̰ų�
	if (nullptr != curAnimation_ &&
		// �����ٲٱⰡ �ƴϰ�
		false == _isForced &&
		// �ִϸ��̼� �̸��� ������
		curAnimation_->GetName() == _name)
	{
		return; // �ϰ͵� �� ���ְ� �Լ��� ������.
	}

	curAnimation_ = FindSpriteAnimation(_name); // ã�� ���� �ִϸ��̼��� ã��...
	curAnimation_->Reboot(); // ���� ���� �������ش�.

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
	// ���� : interTime_[0], interTime_[1] �� 1��°, 2��° �������� ���� ���̴�.
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