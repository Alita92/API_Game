#pragma once
#include "EngineENUM.h"
#include <EngineGeneralDebug.h>
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <list>
#include "Scene.h"

// 분류 : 모든 화면의 개체들의 기본 개념
// 용도 : 타이틀 로고부터 플레이어까지 모든 객체의 기본 개념
// 설명 :

class Scene;
class SpriteRenderer;
class GameCollision;
class Object : public EngineGeneralNameBase
{
private:
	friend Scene;

private:
	Scene* parentScene_;
	int OrderUpdate_;
	int OrderRender_;

private:
	void SetScene(Scene* _parentScene)
	{
		parentScene_ = _parentScene;
	}

public:
	Scene* GetScene()
	{
		if (nullptr == parentScene_)
		{
			EngineGeneralDebug::GameDestroyer();
			return nullptr;
		}

		return parentScene_;
	}

public:
	void SetUpdateOrder(int _UpdateOrder)
	{
		OrderUpdate_ = _UpdateOrder;
	}

	void SetRenderOrder(int _RenderOrder)
	{
		OrderRender_ = _RenderOrder;
	}


private:	// member Var
	int ObjectStyle_; // 종족값
	float4 pos_;

public:
	float4 GetPos()
	{
		return pos_;
	}

	float4 GetCameraLinkedPos()
	{
		return GetPos() - parentScene_->GetCameraPos(); 
	}

	void SetPos(float4 _pos)
	{
		pos_ = _pos;
	}

	void SetMove(float4 _pos)
	{
		pos_ += _pos;
	}


protected:		
	Object(); // default constructer 디폴트 생성자
	~Object() override; // default destructer 디폴트 소멸자

public:		// delete constructer
	Object(const Object& _other) = delete; // default Copy constructer 디폴트 복사생성자
	Object(Object&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	Object& operator=(const Object& _other) = delete; // default Copy operator 디폴트 대입 연산자
	Object& operator=(const Object&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	virtual void LastCheck();

	virtual void UpdateBefore();
	virtual void Update();
	virtual void UpdateAfter();
	virtual void Render();
	virtual void Collision();

private:
	std::list<SpriteRenderer*> spriteRendererList_;

public:
	SpriteRenderer* CreateSpriteRenderer(std::string _ImageName);


	// 충돌
private:
	std::list<GameCollision*> collisionList_;

public:

	template<typename T>
	GameCollision* CreateCollision(T _Group, ColliderType _type)
	{
		return CreateCollision(static_cast<int>(_Group), _type);
	}

	GameCollision* CreateCollision(int _group, ColliderType _type);

};

