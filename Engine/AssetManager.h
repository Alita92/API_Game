#pragma once
#include <map>
#include <string>
#include <list>
#include <Windows.h>


// 분류 :
// 용도 :
// 설명 :
class SceneManager;
class ImageAsset;
class EngineScreen;
class AssetManager
{
public:
	friend SceneManager;
	friend EngineScreen;
private:	// member Var
	static AssetManager* assetManager_;

public:
	static AssetManager& GetAssetManager()
	{
		return *assetManager_;
	}

	static void DestroyMemory()
	{
		if (nullptr != assetManager_)
		{
			delete assetManager_;
			assetManager_ = nullptr;
		}
	}

private:
	ImageAsset* screenImage_;
	ImageAsset* nextBufferImage_;

private:
	void InitializeScreenImage(HDC _screenBackgroundDC);
	void DoubleBuffering();

public:
	ImageAsset* GetNextBuffer();

private : 
	std::map<std::string, ImageAsset*> imageMap_;

public:
	ImageAsset* ImportImage(std::string _name, std::string _path);
	ImageAsset* FindImportedImage(std::string _name);

private:		
	AssetManager(); // default constructer 디폴트 생성자
	~AssetManager(); // default destructer 디폴트 소멸자

public:		// delete constructer
	AssetManager(const AssetManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	AssetManager(AssetManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	AssetManager& operator=(const AssetManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	AssetManager& operator=(const AssetManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
};

