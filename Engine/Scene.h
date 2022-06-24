#pragma once
#include <EngineGeneralDebug.h>
#include <string>
#include <list>
#include <map>
#include <EngineGeneralNameBase.h>
#include <EngineGeneralMath.h>

// 분류 : 생성될 Scene 의 부모 클래스
// 용도 : 자식 Scene 클래스에게 상속될 Scene 의 정의
// 설명 : 

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
	float4 GetCameraPos() // 모작 내에서 카메라는 화면의 좌상단{0,0}
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
	Scene(); // default constructer 디폴트 생성자
	~Scene() override; // default destructer 디폴트 소멸자

public:		// delete constructer
	Scene(const Scene& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Scene(Scene&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	Scene& operator=(const Scene& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Scene& operator=(const Scene&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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

		newObject->SetName("Actor"); // 질문, ObjectType 가 들어가면 될까?
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

