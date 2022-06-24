#pragma once
#include <map>
#include <string>
#include <list>
#include <Windows.h>


// �з� :
// �뵵 :
// ���� :
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
	AssetManager(); // default constructer ����Ʈ ������
	~AssetManager(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	AssetManager(const AssetManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	AssetManager(AssetManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	AssetManager& operator=(const AssetManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	AssetManager& operator=(const AssetManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

