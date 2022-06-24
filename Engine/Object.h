#pragma once
#include "EngineENUM.h"
#include <EngineGeneralDebug.h>
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <list>
#include "Scene.h"

// �з� : ��� ȭ���� ��ü���� �⺻ ����
// �뵵 : Ÿ��Ʋ �ΰ���� �÷��̾���� ��� ��ü�� �⺻ ����
// ���� :

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
	int ObjectStyle_; // ������
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
	Object(); // default constructer ����Ʈ ������
	~Object() override; // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	Object(const Object& _other) = delete; // default Copy constructer ����Ʈ ���������
	Object(Object&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	Object& operator=(const Object& _other) = delete; // default Copy operator ����Ʈ ���� ������
	Object& operator=(const Object&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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


	// �浹
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

