#pragma once
#include <string>
#include <vector>
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <map>

// 분류 : Object 에셋의 관리를 받는 모듈 class
// 용도 : 객체화된 Object class 의 이미지 랜더링을 담당합니다.
// 설명 :

enum class PIVOTENUM
{
	MIDDLE,
	BOTTOM,
	LEFTTOP,
	MAX,

};


class Object;
class ImageAsset;
class SpriteRenderer : public EngineGeneralNameBase
{
private:
	class SpriteAnimation : public EngineGeneralNameBase
	{
		friend SpriteRenderer;

	private:
		int startFrame_;
		int endFrame_;
		int curFrame_;

		std::vector<float>interTime_;
		float curInterTime_;
		SpriteRenderer* linkedRenderer_;
		ImageAsset* linkedImage_;
		PIVOTENUM pivotEnum_;
		bool isLoop_;
		bool isOver_;	

	private:
		SpriteAnimation();

	private:
		void Reboot()
		{
			curFrame_ = startFrame_;
			curInterTime_ = interTime_[0];
			isOver_ = false;
		}

		void UpdateAnimation();
	};

private:
	friend Object;

private:	// member Var
	Object* object_;
	float4 imagePivot_; // 가져 온 스프라이트의 중심점
	float4 pivotpos_; // 
	float4 rendersize_; // 랜더링의 비율
	float4 imagepos_; // 가져온 스프라이트의 좌상단 좌표
	float4 imagesize_; // 가져온 스프라이트의 우하단 좌표
	ImageAsset* image_; // 가져온 스프라이트
	int alpha_;
	bool isCameraLinked_;


	// 나에게 메모 : 변수명 완전히 이해 후 바꿀 것!!!!
public:
private:
	void SetObject(Object* _parent)
	{
		object_ = _parent;
	}

public:
	void SetAlpha(const int _value)
	{
		alpha_ = _value;

		if (255 <= alpha_)
		{
			alpha_ = 255;
			return;
		}

		if (0 >= alpha_)
		{
			alpha_ = 0;
			return;
		}
	}

	void SetAddAlpha(const int _value)
	{
		alpha_ += _value;

		if (255 <= alpha_)
		{
			alpha_ = 255;
			return;
		}

		if (0 >= alpha_)
		{
			alpha_ = 0;
			return;
		}

	}

public:
	void SetImagePivot(const float4& _Value)
	{
		imagePivot_ = _Value;
	}

	void SetPivotPos(const float4& _Value)
	{
		pivotpos_ = _Value;
	}
	void SetRenderSize(const float4& _Value)
	{
		rendersize_ = _Value;
	}
	void SetImagePos(const float4& _Value)
	{
		imagepos_ = _Value;
	}

	void SetImageMove(const float4& _Value)
	{
		imagepos_ += _Value;
	}

	void SetImageSize(const float4& _Value)
	{
		imagesize_ = _Value;
	}

	void SetCameraEffectOn()
	{
		isCameraLinked_ = true;
	}

	void SetCameraEffectOff()
	{
		isCameraLinked_ = false;
	}

	void SetImageAsset(const std::string& _Name);

	void SetImageAsset(ImageAsset* _image)
	{
		image_ = _image;
	}

	ImageAsset* GetImageAsset()
	{
		return image_;
	}

public:
	SpriteRenderer(); // default constructer 디폴트 생성자
	~SpriteRenderer(); // default destructer 디폴트 소멸자

public:		// delete constructer
	SpriteRenderer(const SpriteRenderer& _other) = delete; // default Copy constructer 디폴트 복사생성자
	SpriteRenderer(SpriteRenderer&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	SpriteRenderer& operator=(const SpriteRenderer& _other) = delete; // default Copy operator 디폴트 대입 연산자
	SpriteRenderer& operator=(const SpriteRenderer&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void SetSpriteIndex(size_t _Index, PIVOTENUM _pivotStyle = PIVOTENUM::MIDDLE);
	void StartRender();

private:
	std::map<std::string, SpriteAnimation*> mapAnimation_;
	SpriteAnimation* curAnimation_;


private:
	SpriteAnimation* FindSpriteAnimation(const std::string& _name);


public:
	void CreateSpriteAnimation(const std::string& _name, const std::string& _imageName, int _startFrameOrder, int _endFrameOrder,
		bool _isLoop = true, float _delayTime = 0.1f,PIVOTENUM _pivotEnum = PIVOTENUM::MIDDLE);

public:
//	void FinalCharacterAnimationUpdate(); // (4:3 캐릭터 비율 유지용 함수 : FightScene 플레이어 캐릭터 스프라이트 한정 사용)

public:
	bool IsCurSpriteAnimationName(const std::string& _Name);
	void ChangeSpriteAnimation(const std::string& _name, bool _isForced = false);
	void SetChangeFrame(const std::string& _Name, int _index, float _Time);
	void FinalAnimationUpdate();
	bool IsCurSpriteAnimationEnd();
	int GetCurAnimationFrame(); 
};

