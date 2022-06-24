#include "SceneManager.h"
#include "Scene.h"
#include "AssetManager.h"
#include "GameCollision.h"
#include <EngineGeneralTime.h>
#include "EngineScreen.h"


SceneManager* SceneManager::sceneManager_ = new SceneManager();


SceneManager::SceneManager()
	: mapScene_(), curScene_(nullptr), nextScene_(nullptr)
{
}

SceneManager::~SceneManager()
{
	std::map < std::string, Scene*>::iterator startIter = mapScene_.begin();
	std::map<std::string, Scene*>::iterator endIter = mapScene_.end();

	for (; startIter != endIter; ++startIter)
	{
		delete startIter->second;
		startIter->second = nullptr;
	}
	mapScene_.clear();
}

SceneManager::SceneManager(SceneManager&& _other) noexcept
	: mapScene_(), curScene_(nullptr)
{

}


void SceneManager::ChangeScene(std::string _sceneName, bool _reset)
{
	std::map<std::string, Scene*>::iterator finder = mapScene_.find(_sceneName);

	if (mapScene_.end() == finder)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	GameCollision::ColCheckFuncInitialize(); 
	curScene_ = finder->second;

	if (true == _reset)
	{
		nextScene_->Reset();
	}
}

void SceneManager::Update()
{
	if (nullptr != nextScene_)
	{
		if (nullptr == curScene_)
		{
			curScene_ = nextScene_;
		}
		else
		{
			curScene_ = nextScene_;
		}
		nextScene_ = nullptr;
		EngineGeneralTime::GetEngineGeneralTime().ResetTime();
	}

	if (true == curScene_->isReset_)
	{
		curScene_->Clear();
		curScene_->Loading();
		curScene_->isReset_ = false;
		EngineGeneralTime::GetEngineGeneralTime().ResetTime();
	}


	curScene_->CheckUpdateOrder();
	curScene_->CheckRenderOrder();
	curScene_->CollisionOrderCheck();

	curScene_->SceneUpdateBefore();
	curScene_->UpdateBefore();

	curScene_->SceneUpdate();
	curScene_->Update();

	curScene_->SceneUpdateAfter();
	curScene_->UpdateAfter();
	
	if (false == EngineScreen::GetScreen().GetMoviePlayer()->IsPlay())
	{
		curScene_->Render();
		curScene_->SceneRenderAfter();
		AssetManager::GetAssetManager().DoubleBuffering();
	}

	curScene_->Collision();
	curScene_->Release();
}


void SceneManager::ChangeSceneCheck()
{

}