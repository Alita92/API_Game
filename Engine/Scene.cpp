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

Scene::~Scene() // std::list 의 동적 할당 삭제
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

std::list<GameCollision*>& Scene::GetCollisionList(int _group) // 특정 그룹에 속한 충돌 리스트를 가져온다.
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
				// 이게 다르다는것은
				// 이녀석이 자신의 순서를 바꾸겠다는 이야기가 된다.
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
		// 지워주는 부분
		// 다른 녀석들을 지워준다.
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
	std::map<int, std::list<Object*>>::iterator UpdateStart = objectUpdateMapList_.begin(); // 업데이트 맵 이터레이터를 돌면서...
	std::map<int, std::list<Object*>>::iterator UpdateEnd = objectUpdateMapList_.end();

	for (; UpdateStart != UpdateEnd; ++UpdateStart) // 내부의 
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
		//  새로운 그룹을 만든다.
		std::map<int, std::list<Object*>>::iterator RenderStart = objectRenderMapList_.begin();
		std::map<int, std::list<Object*>>::iterator RenderEnd = objectRenderMapList_.end();
		for (; RenderStart != RenderEnd; ++RenderStart)
		{
			std::list<Object*>::iterator Start = RenderStart->second.begin();
			std::list<Object*>::iterator End = RenderStart->second.end();
			for (; Start != End; ++Start)
			{
				// 이게 다르다는것은
				// 이녀석이 자신의 순서를 바꾸겠다는 이야기가 된다.
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

void Scene::CollisionOrderCheck() // groupIndex 값을 토대로 정리한다.
{
	{
		//  새로운 그룹을 만든다.
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
				{	// 만약 groupindex <=> 키값 연계가 되어있지 않다면...
					collisionListMap_.insert(std::map<int, std::list<GameCollision*>>::value_type((*Start)->GetGroupIndex(), std::list<GameCollision*>()));
					// 그 groupIndex 값을 키값으로 해서 콜리젼을 자료구조에 넣어 준다.
				}

				std::map<int, std::list<GameCollision*>>::iterator RenderIter = collisionListMap_.find((*Start)->GetGroupIndex());
				RenderIter->second.push_back((*Start));

			}
		}
	}

	{
		// 지워주는 부분
		// 다른 녀석들을 지워준다.
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




void Scene::PushCollision(GameCollision* _collision) // 이미 만들어진 충돌 객체를 맵에 넣어줍니다. 키값은 0 입니다.
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

void Scene::Release() // isDeath 가 체크된 함수들을 다 지워준다.
{
	{ // 충돌 판정 삭제
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

	{ // 랜더링 삭제
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