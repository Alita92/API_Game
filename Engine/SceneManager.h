	#pragma once
#include <map>
#include <string>
#include <EngineGeneralDebug.h>

// �з� : �Ŵ���(���� Ŭ����)
// �뵵 : ���� ���� �� ����
// ���� : ���� ����� ��ȯ������ �� �ִ� �� �ڷᱸ������ ����Ѵ�.

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
	SceneManager(); // default constructer ����Ʈ ������
	~SceneManager(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	SceneManager(const SceneManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	SceneManager(SceneManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	SceneManager& operator=(const SceneManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	SceneManager& operator=(const SceneManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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
		mapScene_.insert(std::map<std::string, Scene*>::value_type(_sceneName, newScene)); // �ʿ� ���ο� ���� ��ϻ����Ѵ�.
	}

	void ChangeScene(std::string _sceneName, bool _reset = false);

private:
	void ChangeSceneCheck();
};

