#include "Object.h"
#include "SpriteRenderer.h"
#include "GameCollision.h"


// Static Var
// Static Func

// constructer destructer
Object::Object()
	: ObjectStyle_(-1), OrderUpdate_(0), OrderRender_(0), parentScene_(nullptr)
{
}

Object::~Object()
{
	std::list<SpriteRenderer*>::iterator start = spriteRendererList_.begin();
	std::list<SpriteRenderer*>::iterator end = spriteRendererList_.end();

	for (; start != end; ++start)
	{
		if (nullptr == (*start))
		{
			continue;
		}

		delete (*start);
		(*start) = nullptr;
	}
	spriteRendererList_.clear();

	{
		std::list<GameCollision*>::iterator StartIter = collisionList_.begin();
		std::list<GameCollision*>::iterator EndIter = collisionList_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr == (*StartIter))
			{
				continue;
			}
			delete (*StartIter);
			(*StartIter) = nullptr;
		}
		collisionList_.clear();
	}
}

Object::Object(Object&& _other) noexcept	
	: ObjectStyle_(-1), OrderUpdate_(0), OrderRender_(0), parentScene_(nullptr)
{
}

//member Func
void Object::LastCheck()
{

}

void Object::UpdateBefore()
{

}

void Object::Update()
{

}

void Object::UpdateAfter()
{

}

void Object::Render()
{

}

void Object::Collision()
{

}

SpriteRenderer* Object::CreateSpriteRenderer(std::string _ImageName)
{
	SpriteRenderer* newSpriteRenderer = new SpriteRenderer();
	newSpriteRenderer->SetParent(this);
	newSpriteRenderer->SetObject(this);
	newSpriteRenderer->SetImageAsset(_ImageName);
	spriteRendererList_.push_back(newSpriteRenderer);

	return newSpriteRenderer;
}

GameCollision* Object::CreateCollision(int _group, ColliderType _type)
{
	GameCollision* newGameCollision = new GameCollision();
	newGameCollision->SetParent(this);
	newGameCollision->SetObject(this);
	newGameCollision->SetColType(_type);
	newGameCollision->SetGroupIndex(_group);
	collisionList_.push_back(newGameCollision);

	GetScene()->PushCollision(newGameCollision);

	return newGameCollision;
}

