#pragma once
#include <string>
#include <Windows.h>
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <EngineGeneralDebug.h>
#include <vector>

// 분류 :
// 용도 :
// 설명 :
class AssetManager;
class ImageAsset : public EngineGeneralNameBase
{
private:
	friend AssetManager;

private:	// member Var
	HBITMAP imageAssetHandle_;
	HBITMAP prevHandle_;
	BITMAP imageAssetInfo_;

	HDC imageAssetDc_;

public:
	std::vector<float4> cutImageLeftTop_;
	std::vector<float4> cutImageRightBottom_;

public:
	bool isCut()
	{
		return cutImageLeftTop_.size() != 0;
	}

	float4 GetCutLeftTop(size_t _Index)
	{
		if (_Index >= cutImageLeftTop_.size())
		{
			EngineGeneralDebug::GameDestroyer();
			return float4::ZERO;
		}

		return cutImageLeftTop_[_Index];
	}
	float4 GetCutRightBottom(size_t _Index)
	{
		if (_Index >= cutImageRightBottom_.size())
		{
			EngineGeneralDebug::GameDestroyer();
			return float4::ZERO;
		}
		return cutImageRightBottom_[_Index];
	}

	HDC GetDC()
	{
		return imageAssetDc_;
	}

	float4 GetImageSize()
	{
		return { static_cast<float>(imageAssetInfo_.bmWidth), static_cast<float>(imageAssetInfo_.bmHeight) };
	}

private:
	ImageAsset(); // default constructer 디폴트 생성자
	~ImageAsset(); // default destructer 디폴트 소멸자

public:		// delete constructer
	ImageAsset(const ImageAsset& _other) = delete; // default Copy constructer 디폴트 복사생성자
	ImageAsset(ImageAsset&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	ImageAsset& operator=(const ImageAsset& _other) = delete; // default Copy operator 디폴트 대입 연산자
	ImageAsset& operator=(const ImageAsset&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void CutImage(const float4 _cut);

public:
	bool GetCompatibleDC(HDC _dc);
	bool GetCompatibleDC(HDC _dc, const float4& _size);

public:		//member Func
	bool Import(std::string _path);

private:
	void BmpInfoCheck();
public:
	void MyBitCopy(ImageAsset* _CopyImage, const float4& _LeftTopPos, const float4& _Size, const float4& _ImagePos = float4::ZERO);
	void MyBitCopyToImageSize(ImageAsset* _CopyImage, const float4& _LeftTopPos, const float4& _ImagePos = float4::ZERO);
	void MyBitCopyToImageSizeToCenter(ImageAsset* _CopyImage, const float4& _Pos, const float4& _ImagePos = float4::ZERO);
	void MyBitCopyToImageSizeToBot(ImageAsset* _CopyImage, const float4& _Pos, const float4& _ImagePos = float4::ZERO);

	void MyTransBitCopy(ImageAsset* _copyImage, const float4& _leftTopPos, const float4 _renderSize, const float4& _imagePos, const float4& _imageSize, int _invisibleColor);
	DWORD GetColor4Byte(int _x, int _y);
	void MyAlphaBitCopy(ImageAsset* _copyImage, const int _alpha, const float4& _leftTopPos, const float4& _size, const float4& _imagePos, const float4& _imageSize);

};

