#pragma once
#include <string>
#include <vector>
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <map>

// �з� : Object ������ ������ �޴� ��� class
// �뵵 : ��üȭ�� Object class �� �̹��� �������� ����մϴ�.
// ���� :

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
	float4 imagePivot_; // ���� �� ��������Ʈ�� �߽���
	float4 pivotpos_; // 
	float4 rendersize_; // �������� ����
	float4 imagepos_; // ������ ��������Ʈ�� �»�� ��ǥ
	float4 imagesize_; // ������ ��������Ʈ�� ���ϴ� ��ǥ
	ImageAsset* image_; // ������ ��������Ʈ
	int alpha_;
	bool isCameraLinked_;


	// ������ �޸� : ������ ������ ���� �� �ٲ� ��!!!!
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
	SpriteRenderer(); // default constructer ����Ʈ ������
	~SpriteRenderer(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	SpriteRenderer(const SpriteRenderer& _other) = delete; // default Copy constructer ����Ʈ ���������
	SpriteRenderer(SpriteRenderer&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	SpriteRenderer& operator=(const SpriteRenderer& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SpriteRenderer& operator=(const SpriteRenderer&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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
//	void FinalCharacterAnimationUpdate(); // (4:3 ĳ���� ���� ������ �Լ� : FightScene �÷��̾� ĳ���� ��������Ʈ ���� ���)

public:
	bool IsCurSpriteAnimationName(const std::string& _Name);
	void ChangeSpriteAnimation(const std::string& _name, bool _isForced = false);
	void SetChangeFrame(const std::string& _Name, int _index, float _Time);
	void FinalAnimationUpdate();
	bool IsCurSpriteAnimationEnd();
	int GetCurAnimationFrame(); 
};

