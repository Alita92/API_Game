	#pragma once
#include <map>
#include <string>
#include <EngineGeneralDebug.h>

// 분류 : 매니저(관리 클래스)
// 용도 : 씬의 관리 및 생성
// 설명 : 씬을 만들고 전환시켜줄 수 있는 맵 자료구조형을 사용한다.

class Scene;
class SceneManager
{
private:	// member Var

private:
	static SceneManager* sceneManager_;

public:
	static SceneManager& GetSceneManager()
	{
		return *sceneManager_;
	}

	static void DestroyMemory()
	{
		if (nullptr != sceneManager_)
		{
			delete sceneManager_;
			sceneManager_ = nullptr;
		}
	}

private:
	std::map<std::string, Scene*> mapScene_;
	Scene* curScene_;
	Scene* nextScene_;

public:
	void Update();

private:		
	SceneManager(); // default constructer 디폴트 생성자
	~SceneManager(); // default destructer 디폴트 소멸자

public:		// delete constructer
	SceneManager(const SceneManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	SceneManager(SceneManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	SceneManager& operator=(const SceneManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	SceneManager& operator=(const SceneManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	template <typename SceneType>
	void CreateScene(std::string _sceneName)
	{
		if (mapScene_.end() != mapScene_.find(_sceneName))
		{
			EngineGeneralDebug::GameDestroyer();
			return;
		}

		Scene* newScene = new SceneType();
		newScene->SceneLoading();
		newScene->SetName(_sceneName);
		mapScene_.insert(std::map<std::string, Scene*>::value_type(_sceneName, newScene)); // 맵에 새로운 씬을 등록삽입한다.
	}

	void ChangeScene(std::string _sceneName, bool _reset = false);

private:
	void ChangeSceneCheck();
};

