#include "Scene.h"
#include "Object.h"
#include "GameCollision.h"

// Static Var
// Static Func

// constructer destructer
Scene::Scene()
	: isLoading_(false), cameraPos_(float4::ZERO)
{
	objectUpdateMapList_.insert(std::map<int, std::list<Object*>>::value_type(0, std::list<Object*>()));
	objectRenderMapList_.insert(std::map<int, std::list<Object*>>::value_type(0, std::list<Object*>()));
	collisionListMap_.insert(std::map<int, std::list<GameCollision*>>::value_type(0, std::list<GameCollision*>()));
}

Scene::~Scene() // std::list �� ���� �Ҵ� ����
{
	//{
	//	std::map<int, std::list<Object*>>::iterator updateStart = objectUpdateMapList_.begin();
	//	std::map<int, std::list<Object*>>::iterator updateEnd = objectUpdateMapList_.end();
	//
	//	for (; updateEnd != updateStart; ++updateStart)
	//	{
	//		std::list<Object*>::iterator start = updateStart->second.begin();
	//		std::list<Object*>::iterator end = updateStart->second.end();
	//
	//		for (; start != end; ++start)
	//		{
	//			delete (*start);
	//			(*start) = nullptr;
	//		}
	//	}
	//	objectUpdateMapList_.clear();
	//}
	Clear();
}

Scene::Scene(Scene&& _other) noexcept
{
}

//member Func

std::list<GameCollision*>& Scene::GetCollisionList(int _group) // Ư�� �׷쿡 ���� �浹 ����Ʈ�� �����´�.
{
	std::map<int, std::list<GameCollision*>>::iterator FindGroupIter
		= collisionListMap_.find(_group);

	if (FindGroupIter == collisionListMap_.end())
	{
		collisionListMap_.insert(std::map<int, std::list<GameCollision*>>::value_type(_group, std::list<GameCollision*>()));
		FindGroupIter = collisionListMap_.find(_group);
	}

	return FindGroupIter->second;
}

Object* Scene::FindObject(const std::string _objectName)
{

	std::map<std::string, Object*>::iterator finder = mapObject_.find(_objectName);

	if (mapObject_.end() == finder)
	{
		EngineGeneralDebug::GameDestroyer();
		return nullptr;
	}

	return finder->second;
}

void Scene::CheckUpdateOrder()
{
	{
		std::map<int, std::list<Object*>>::iterator UpdateStart = objectUpdateMapList_.begin();
		std::map<int, std::list<Object*>>::iterator UpdateEnd = objectUpdateMapList_.end();
		for (; UpdateStart != UpdateEnd; ++UpdateStart)
		{
			std::list<Object*>::iterator Start = UpdateStart->second.begin();
			std::list<Object*>::iterator End = UpdateStart->second.end();
			for (; Start != End; ++Start)
			{
				// �̰� �ٸ��ٴ°���
				// �̳༮�� �ڽ��� ������ �ٲٰڴٴ� �̾߱Ⱑ �ȴ�.
				if (UpdateStart->first == (*Start)->OrderUpdate_)
				{
					continue;
				}

				if (objectUpdateMapList_.end() == objectUpdateMapList_.find((*Start)->OrderUpdate_))
				{
					objectUpdateMapList_.insert(std::map<int, std::list<Object*>>::value_type((*Start)->OrderUpdate_, std::list<Object*>()));
				}

				std::map<int, std::list<Object*>>::iterator UpdateIter = objectUpdateMapList_.find((*Start)->OrderUpdate_);
				UpdateIter->second.push_back((*Start));
			}
		}
	}

	{
		// �����ִ� �κ�
		// �ٸ� �༮���� �����ش�.
		std::map<int, std::list<Object*>>::iterator UpdateStart = objectUpdateMapList_.begin();
		std::map<int, std::list<Object*>>::iterator UpdateEnd = objectUpdateMapList_.end();
		for (; UpdateStart != UpdateEnd; ++UpdateStart)
		{
			std::list<Object*>::iterator Start = UpdateStart->second.begin();
			std::list<Object*>::iterator End = UpdateStart->second.end();
			for (; Start != End; )
			{
				if (UpdateStart->first == (*Start)->OrderUpdate_)
				{
					++Start;
					continue;
				}

				Start = UpdateStart->second.erase(Start);
			}
		}
	}
}

void Scene::UpdateBefore()
{
	std::map<int, std::list<Object*>>::iterator UpdateStart = objectUpdateMapList_.begin();
	std::map<int, std::list<Object*>>::iterator UpdateEnd = objectUpdateMapList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<Object*>::iterator Start = UpdateStart->second.begin();
		std::list<Object*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->UpdateBefore();
		}
	}
}

void Scene::Update()
{
	std::map<int, std::list<Object*>>::iterator UpdateStart = objectUpdateMapList_.begin();
	std::map<int, std::list<Object*>>::iterator UpdateEnd = objectUpdateMapList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<Object*>::iterator Start = UpdateStart->second.begin();
		std::list<Object*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->Update();
		}
	}
}

void Scene::Collision()
{
	std::map<int, std::list<Object*>>::iterator UpdateStart = objectUpdateMapList_.begin(); // ������Ʈ �� ���ͷ����͸� ���鼭...
	std::map<int, std::list<Object*>>::iterator UpdateEnd = objectUpdateMapList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart) // ������ 
	{
		std::list<Object*>::iterator Start = UpdateStart->second.begin();
		std::list<Object*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->Collision();
		}
	}
}

void Scene::UpdateAfter()
{
	std::map<int, std::list<Object*>>::iterator UpdateStart = objectUpdateMapList_.begin();
	std::map<int, std::list<Object*>>::iterator UpdateEnd = objectUpdateMapList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart)
	{
		std::list<Object*>::iterator Start = UpdateStart->second.begin();
		std::list<Object*>::iterator End = UpdateStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->UpdateAfter();
		}
	}
}

void Scene::CheckRenderOrder()
{
	{
		//  ���ο� �׷��� �����.
		std::map<int, std::list<Object*>>::iterator RenderStart = objectRenderMapList_.begin();
		std::map<int, std::list<Object*>>::iterator RenderEnd = objectRenderMapList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<Object*>::iterator Start = RenderStart->second.begin();
			std::list<Object*>::iterator End = RenderStart->second.end();
			for (; Start != End; ++Start)
			{
				// �̰� �ٸ��ٴ°���
				// �̳༮�� �ڽ��� ������ �ٲٰڴٴ� �̾߱Ⱑ �ȴ�.
				if (RenderStart->first == (*Start)->OrderRender_)
				{
					continue;
				}

				if (objectRenderMapList_.end() == objectRenderMapList_.find((*Start)->OrderRender_))
				{
					objectRenderMapList_.insert(std::map<int, std::list<Object*>>::value_type((*Start)->OrderRender_, std::list<Object*>()));
				}

				std::map<int, std::list<Object*>>::iterator RenderIter = objectRenderMapList_.find((*Start)->OrderRender_);
				RenderIter->second.push_back((*Start));
			}
		}
	}

	{
		std::map<int, std::list<Object*>>::iterator RenderStart = objectRenderMapList_.begin();
		std::map<int, std::list<Object*>>::iterator RenderEnd = objectRenderMapList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<Object*>::iterator Start = RenderStart->second.begin();
			std::list<Object*>::iterator End = RenderStart->second.end();
			for (; Start != End; )
			{
				if (RenderStart->first == (*Start)->OrderRender_)
				{
					++Start;
					continue;
				}

				Start = RenderStart->second.erase(Start);
			}
		}
	}

}

void Scene::CollisionOrderCheck() // groupIndex ���� ���� �����Ѵ�.
{
	{
		//  ���ο� �׷��� �����.
		std::map<int, std::list<GameCollision*>>::iterator RenderStart = collisionListMap_.begin();
		std::map<int, std::list<GameCollision*>>::iterator RenderEnd = collisionListMap_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GameCollision*>::iterator Start = RenderStart->second.begin();
			std::list<GameCollision*>::iterator End = RenderStart->second.end();
			for (; Start != End; ++Start)
			{

				if (RenderStart->first == (*Start)->GetGroupIndex())
				{
					continue;
				}

				if (collisionListMap_.end() == collisionListMap_.find((*Start)->GetGroupIndex()))
				{	// ���� groupindex <=> Ű�� ���谡 �Ǿ����� �ʴٸ�...
					collisionListMap_.insert(std::map<int, std::list<GameCollision*>>::value_type((*Start)->GetGroupIndex(), std::list<GameCollision*>()));
					// �� groupIndex ���� Ű������ �ؼ� �ݸ����� �ڷᱸ���� �־� �ش�.
				}

				std::map<int, std::list<GameCollision*>>::iterator RenderIter = collisionListMap_.find((*Start)->GetGroupIndex());
				RenderIter->second.push_back((*Start));

			}
		}
	}

	{
		// �����ִ� �κ�
		// �ٸ� �༮���� �����ش�.
		std::map<int, std::list<GameCollision*>>::iterator RenderStart = collisionListMap_.begin();
		std::map<int, std::list<GameCollision*>>::iterator RenderEnd = collisionListMap_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<GameCollision*>::iterator Start = RenderStart->second.begin();
			std::list<GameCollision*>::iterator End = RenderStart->second.end();
			for (; Start != End; )
			{
				if (RenderStart->first == (*Start)->GetGroupIndex())
				{
					++Start;
					continue;
				}

				Start = RenderStart->second.erase(Start);
			}
		}
	}
}

void Scene::Render()
{
	std::map<int, std::list<Object*>>::iterator RenderStart = objectRenderMapList_.begin();
	std::map<int, std::list<Object*>>::iterator RenderEnd = objectRenderMapList_.end();

	for (; RenderStart != RenderEnd; ++RenderStart)
	{
		std::list<Object*>::iterator Start = RenderStart->second.begin();
		std::list<Object*>::iterator End = RenderStart->second.end();

		for (; Start != End; ++Start)
		{
			(*Start)->Render();
		}
	}
}

void Scene::SceneUpdateBefore()
{

}

void Scene::SceneUpdate()
{

}

void Scene::SceneUpdateAfter()
{

}

void Scene::SceneRenderAfter()
{

}




void Scene::PushCollision(GameCollision* _collision) // �̹� ������� �浹 ��ü�� �ʿ� �־��ݴϴ�. Ű���� 0 �Դϴ�.
{
	std::map<int, std::list<GameCollision*>>::iterator CollisionIter = collisionListMap_.find(0);
	if (collisionListMap_.end() == CollisionIter)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	CollisionIter->second.push_back(_collision);
	return;
}

void Scene::Release() // isDeath �� üũ�� �Լ����� �� �����ش�.
{
	{ // �浹 ���� ����
		std::map<int, std::list<GameCollision*>>::iterator collisionStart = collisionListMap_.begin();
		std::map<int, std::list<GameCollision*>>::iterator collisionEnd = collisionListMap_.end();
		for (; collisionEnd != collisionStart; ++collisionStart)
		{
			std::list<GameCollision*>::iterator listStart = collisionStart->second.begin();
			std::list<GameCollision*>::iterator listEnd = collisionStart->second.end();

			for (; listStart != listEnd;)
			{
				if (false == (*listStart)->IsDeath())
				{
					++listStart;
					continue;
				}
				listStart = collisionStart->second.erase(listStart);
			}
		}
	}

	{ // ������ ����
		std::map<int, std::list<Object*>>::iterator renderStart = objectRenderMapList_.begin();
		std::map<int, std::list<Object*>>::iterator renderEnd = objectRenderMapList_.end();

		for (; renderEnd != renderStart; ++renderStart)
		{
			std::list<Object*>::iterator listStart = renderStart->second.begin();
			std::list<Object*>::iterator listEnd = renderStart->second.end();

			for (; listStart != listEnd;)
			{
				if (true == (*listStart)->IsDeath())
				{
					listStart = renderStart->second.erase(listStart);
				}
				else
				{
					++listStart;
				}
			}
		}
	}

	{
		std::map<int, std::list<Object*>>::iterator updateStart = objectUpdateMapList_.begin();
		std::map<int, std::list<Object*>>::iterator updateEnd = objectUpdateMapList_.end();

		for (; updateEnd != updateStart; ++updateStart)
		{
			std::list<Object*>::iterator listStart = updateStart->second.begin();
			std::list<Object*>::iterator listEnd = updateStart->second.end();

			for (; listStart != listEnd;)
			{
				if (true == (*listStart)->IsDeath())
				{
					delete (*listStart);
					listStart = updateStart->second.erase(listStart);
				}
				else
				{
					++listStart;
				}
			}
		}
	}
}

void Scene::Reset()
{
	isReset_ = true;
}

void Scene::Clear()
{
	std::map<int, std::list<Object*>>::iterator updateStart = objectUpdateMapList_.begin();
	std::map<int, std::list<Object*>>::iterator updateEnd = objectUpdateMapList_.end();

	for (; updateEnd != updateStart; ++updateStart)
	{
		std::list<Object*>::iterator start = updateStart->second.begin();
		std::list<Object*>::iterator end = updateStart->second.end();

		for (; start != end; ++start)
		{
			delete (*start);
			(*start) = nullptr;
		}
	}

	collisionListMap_.clear();
	objectRenderMapList_.clear();
	objectUpdateMapList_.clear();
	mapObject_.clear();

	objectUpdateMapList_.insert(std::map<int, std::list<Object*>>::value_type(0, std::list<Object*>()));
	objectRenderMapList_.insert(std::map<int, std::list<Object*>>::value_type(0, std::list<Object*>()));
	collisionListMap_.insert(std::map<int, std::list<GameCollision*>>::value_type(0, std::list<GameCollision*>()));
	cameraPos_ = float4::ZERO;
}