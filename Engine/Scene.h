#pragma once
#include <EngineGeneralDebug.h>
#include <string>
#include <list>
#include <map>
#include <EngineGeneralNameBase.h>
#include <EngineGeneralMath.h>

// �з� : ������ Scene �� �θ� Ŭ����
// �뵵 : �ڽ� Scene Ŭ�������� ��ӵ� Scene �� ����
// ���� : 

class SceneManager;
class Object;
class GameCollision;
class Scene : public EngineGeneralNameBase
{
private:
	friend SceneManager;
	friend Object;
	friend GameCollision;

private:	// member Var
	bool isReset_;
	bool isLoading_;
	float4 cameraPos_;

	// std::list<Object*> listObject_;
	std::map<int, std::list<Object*>> objectUpdateMapList_;
	std::map<int, std::list<Object*>> objectRenderMapList_;

	std::map<int, std::list<GameCollision*>> collisionListMap_;

	std::map<std::string, Object*> mapObject_;


private:
	std::list<GameCollision*>& GetCollisionList(int _group);

public:
	float4 GetCameraPos() // ���� ������ ī�޶�� ȭ���� �»��{0,0}
	{
		return cameraPos_;
	}

	void MoveCameraPos(float4 _pos)
	{
		cameraPos_ += _pos;
	}

	void SetCameraPos(float4 _pos)
	{
		cameraPos_ = _pos;
	}

private:
	void SceneLoading()
	{
		if (false == GetLoading())
		{
			Loading();
			LoadingEnd();
		}
	}

	bool GetLoading()
	{
		return isLoading_;
	}

	void LoadingEnd()
	{
		isLoading_ = true;
	}



protected:
	Scene(); // default constructer ����Ʈ ������
	~Scene() override; // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	Scene(const Scene& _other) = delete; // default Copy constructer ����Ʈ ���������
	Scene(Scene&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	Scene& operator=(const Scene& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Scene& operator=(const Scene&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	template<typename ObjectType>
	ObjectType* CreateObject() // for std::list
	{
		std::map<int, std::list<Object*>>::iterator updateIter = objectUpdateMapList_.find(0);
		if (objectUpdateMapList_.end() == updateIter)
		{
			EngineGeneralDebug::GameDestroyer();
			return nullptr;
		}

		std::map<int, std::list<Object*>>::iterator renderIter = objectRenderMapList_.find(0);
		if (objectRenderMapList_.end() == renderIter)
		{
			EngineGeneralDebug::GameDestroyer();
			return nullptr;
		}

		ObjectType* newObject = new ObjectType();

		newObject->SetName("Actor"); // ����, ObjectType �� ���� �ɱ�?
		newObject->SetScene(this);
		newObject->LastCheck();

		updateIter->second.push_back(newObject);
		renderIter->second.push_back(newObject);
		return newObject;
	}

	template<typename ObjectType>
	ObjectType* CreateObject(const std::string _objectName)
	{
		ObjectType* newObject = CreateObject<ObjectType>();

		if (mapObject_.end() != mapObject_.find(_objectName))
		{
			EngineGeneralDebug::GameDestroyer();
			return nullptr;
		}

		newObject->SetName(_objectName);

		mapObject_.insert(std::map<std::string, Object*>::value_type(_objectName, newObject));
		return newObject;
	}

	Object* FindObject(const std::string _objectName);

private:
	void PushCollision(GameCollision* _collision);

protected:		//member Func
	virtual void Loading() = 0;

	virtual void SceneUpdateBefore();
	virtual void SceneUpdate();
	virtual void SceneUpdateAfter();
	virtual void SceneRenderAfter();

private:
	void CheckUpdateOrder();
	void CollisionOrderCheck();
	void CheckRenderOrder();
	void UpdateBefore();
	void Update();
	void UpdateAfter();


	void Collision();
	void Render();
	void Release();
	void Clear();

public:
	void Reset();

};

