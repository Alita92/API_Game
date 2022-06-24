#include "ImageAsset.h"

#pragma comment(lib, "msimg32.lib")
// Static Var
// Static Func

// constructer destructer
ImageAsset::ImageAsset()
{
}

ImageAsset::~ImageAsset()
{
}

ImageAsset::ImageAsset(ImageAsset&& _other) noexcept
{
}

void ImageAsset::CutImage(const float4 _cut)
{
	if (0.0f >= _cut.x)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	if (0.0f >= _cut.y)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	size_t xCount = static_cast<size_t>(GetImageSize().x / _cut.x);
	size_t yCount = static_cast<size_t>(GetImageSize().y / _cut.y);

	cutImageLeftTop_.reserve(xCount * yCount);
	cutImageRightBottom_.reserve(xCount * yCount);

	float4 StartImageLeftTop = float4::ZERO;
	float4 StartImageRightBottom = _cut;

	for (size_t i = 0; i < yCount; i++)
	{
		for (size_t i = 0; i < xCount; i++)
		{
			// 0,0
			cutImageLeftTop_.push_back(StartImageLeftTop);
			// 128, 128
			cutImageRightBottom_.push_back(StartImageRightBottom);

			// 0             +  128
			StartImageLeftTop.x += StartImageRightBottom.x;
		}

		StartImageLeftTop.x = 0.0f;
		StartImageLeftTop.y += StartImageRightBottom.y;
	}
}

bool ImageAsset::GetCompatibleDC(HDC _dc)
{
	if (nullptr == _dc)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	imageAssetDc_ = _dc;
	BmpInfoCheck();
	return true;
}

bool ImageAsset::GetCompatibleDC(HDC _dc, const float4& _size)
{
	imageAssetHandle_ = CreateCompatibleBitmap(_dc, _size.ix(), _size.iy());

	imageAssetDc_ = CreateCompatibleDC(nullptr);
	HANDLE prevHandle = SelectObject(imageAssetDc_, static_cast<HGDIOBJ>(imageAssetHandle_));
	prevHandle_ = static_cast<HBITMAP>(prevHandle);
	BmpInfoCheck();
	return true;
}

//member Func
bool ImageAsset::Import(std::string _path) // 실제 이미지 파일을 하드에서 가져오는 함수입니다.
{
	HANDLE handleBuffer = nullptr;
	handleBuffer = LoadImageA(nullptr, _path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	imageAssetHandle_ = static_cast<HBITMAP>(handleBuffer);

	if (nullptr == imageAssetHandle_)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	imageAssetDc_ = CreateCompatibleDC(nullptr);
	HANDLE prevHandleBuffer = SelectObject(imageAssetDc_, static_cast<HGDIOBJ>(imageAssetHandle_));
	prevHandle_ = static_cast<HBITMAP>(prevHandleBuffer);

	BmpInfoCheck();

	return true;
}

void ImageAsset::BmpInfoCheck()
{
	if (nullptr == imageAssetDc_)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	HBITMAP bmpChecker = static_cast<HBITMAP>(GetCurrentObject(imageAssetDc_, OBJ_BITMAP));
	GetObjectA(bmpChecker, sizeof(BITMAP), &imageAssetInfo_);
}

void ImageAsset::MyBitCopy(ImageAsset* _CopyImage, const float4& _LeftTopPos, const float4& _Size, const float4& _ImagePos)
{
	BitBlt(imageAssetDc_,
		_LeftTopPos.ix(),
		_LeftTopPos.iy(),
		_Size.ix(),
		_Size.iy(),
		_CopyImage->imageAssetDc_,
		_ImagePos.ix(),
		_ImagePos.iy(),
		SRCCOPY);
}

void ImageAsset::MyBitCopyToImageSize(ImageAsset* _CopyImage, const float4& _LeftTopPos, const float4& _ImagePos)
{
	MyBitCopy(_CopyImage, _LeftTopPos, _CopyImage->GetImageSize(), _ImagePos);
}

void ImageAsset::MyBitCopyToImageSizeToCenter(ImageAsset* _CopyImage, const float4& _Pos, const float4& _ImagePos)
{
	MyBitCopy(_CopyImage, _Pos - _CopyImage->GetImageSize().halffloat4(), _CopyImage->GetImageSize(), _ImagePos);
}

void ImageAsset::MyBitCopyToImageSizeToBot(ImageAsset* _CopyImage, const float4& _Pos, const float4& _ImagePos)
{
	float4 pivotBuffer = _CopyImage->GetImageSize().halffloat4();
	pivotBuffer.y = _CopyImage->GetImageSize().y;
	MyBitCopy(_CopyImage, _Pos - pivotBuffer, _CopyImage->GetImageSize(), _ImagePos);
}


void ImageAsset::MyTransBitCopy(ImageAsset* _copyImage, const float4& _leftTopPos, const float4 _renderSize, const float4& _imagePos, const float4& _imageSize, int _invisibleColor)
{
	if (nullptr == _copyImage)
	{
		EngineGeneralDebug::MsgBoxError("이미지 세팅 에러 : MyTransBitCopy 에 들어온 _copyImage 인자가 Null입니다.");
	}
	TransparentBlt(
		imageAssetDc_,
		_leftTopPos.ix(),
		_leftTopPos.iy(),
		_renderSize.ix(),
		_renderSize.iy(),
		_copyImage->imageAssetDc_,
		_imagePos.ix(),
		_imagePos.iy(),
		_imageSize.ix(),
		_imageSize.iy(),
		_invisibleColor);

}

void ImageAsset::MyAlphaBitCopy(ImageAsset* _copyImage, 
	const int _alpha, const float4& _leftTopPos, const float4& _size, const float4& _imagePos, const float4& _imageSize)
{
	BLENDFUNCTION blend;

	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = _alpha;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(
		imageAssetDc_,
		_leftTopPos.ix(),
		_leftTopPos.iy(),
		_size.ix(),
		_size.iy(),
		_copyImage->imageAssetDc_,
		_imagePos.ix(),
		_imagePos.iy(),
		_imageSize.ix(),
		_imageSize.iy(),
		blend
	);
}


DWORD ImageAsset::GetColor4Byte(int _x, int _y)
{
	return GetPixel(imageAssetDc_, _x, _y);
}