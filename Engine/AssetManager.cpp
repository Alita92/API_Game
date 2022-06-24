#include <EngineGeneralDebug.h>

#include "EngineScreen.h"
#include "AssetManager.h"
#include "ImageAsset.h"

AssetManager* AssetManager::assetManager_ = new AssetManager();



// constructer destructer
AssetManager::AssetManager()
{
}


AssetManager::~AssetManager()
{
	std::map<std::string, ImageAsset*>::iterator start = imageMap_.begin();
	std::map<std::string, ImageAsset*>::iterator end = imageMap_.end();

	for (; end != start; ++start)
	{
		delete start->second;
		start->second = nullptr;
	}
	imageMap_.clear();

	if (nullptr != nextBufferImage_)
	{
		delete nextBufferImage_;
		nextBufferImage_ = nullptr;
	}

	if (nullptr != screenImage_)
	{
		delete screenImage_;
		screenImage_ = nullptr;
	}
}

AssetManager::AssetManager(AssetManager&& _other) noexcept
{
	
}

void AssetManager::DoubleBuffering()
{
	screenImage_->MyBitCopy(nextBufferImage_, float4::ZERO, EngineScreen::GetScreen().GetSize());
}

ImageAsset* AssetManager::GetNextBuffer()
{
	return nextBufferImage_;
}

void AssetManager::InitializeScreenImage(HDC _screenBackgroundDC)
{
	if (nullptr == _screenBackgroundDC)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}
	nextBufferImage_ = new ImageAsset();
	nextBufferImage_->GetCompatibleDC(_screenBackgroundDC, EngineScreen::GetScreen().GetSize());

	screenImage_ = new ImageAsset();
	screenImage_->GetCompatibleDC(_screenBackgroundDC);
}

//member Func
ImageAsset* AssetManager::FindImportedImage(std::string _name)
{
	std::map<std::string, ImageAsset*>::iterator finder = imageMap_.find(_name);
	if (imageMap_.end() == finder)
	{
		return nullptr;
	}

	return finder->second;

}

ImageAsset* AssetManager::ImportImage(std::string _name, std::string _path)
{
	if (nullptr != FindImportedImage(_name))
	{
		EngineGeneralDebug::GameDestroyer();
		return nullptr;
	}

	ImageAsset* newImage = new ImageAsset();
	if (false == newImage->Import(_path))
	{
		delete newImage;
		EngineGeneralDebug::GameDestroyer();
		return nullptr;
	}

	imageMap_.insert(std::map<std::string, ImageAsset*>::value_type(_name, newImage));
	return newImage;
}


