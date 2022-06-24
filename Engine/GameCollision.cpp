#include "GameCollision.h"
#include <EngineGeneralDebug.h>
#include "AssetManager.h"
#include "ImageAsset.h"
#include "Object.h"
#include "Scene.h"


// Static Var
// Static Func

// constructer destructer
GameCollision::GameCollision()
	: colType_(ColliderType::MAX) , groupIndex_(4), object_(nullptr), CheckColor(0), imageptr_(nullptr), isCameraRelated_(false), collisionCount_(-1)
{
}

GameCollision::~GameCollision()
{
}

float4 GameCollision::GetCollisionPos()
{
	return object_->GetPos() + pivot_;
}

GameCollision::GameCollision(GameCollision&& _other) noexcept
	: colType_(ColliderType::MAX), groupIndex_(_other.groupIndex_), object_(_other.object_), CheckColor(_other.CheckColor), imageptr_(_other.imageptr_), isCameraRelated_(_other.isCameraRelated_)
{
}

//member Func
bool GameCollision::CollisionCheck(GameCollision* _other) // 실제 This 의 Collision 과 타 collision 충돌을 체크하는 함수
{
	if (0 == collisionCount_)
	{
		return false;
	}

	if (nullptr == this)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	if (nullptr == _other)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	if (nullptr == _other)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	// 현 게임내에서는 RectToRect 로 들어가야함
	if (true == arrFunc_[GetTypeToIndex()][_other->GetTypeToIndex()](this, _other))
	{
		if (0 < collisionCount_)
		{
			--collisionCount_;
		}

		return true;
	}

	return false;
}


void GameCollision::SetImportedImage(std::string _ImageName)
{
	ImageAsset* ImagePtr = AssetManager::GetAssetManager().FindImportedImage(_ImageName);

	if (nullptr == ImagePtr)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	imageptr_ = ImagePtr;
}

GameCollision* GameCollision::CollisionGroupCheckSingle(int _otherIndex)
{
	// 상대 그룹을 얻어와야 일단 뭔가를 시작할수 있다.
	if (nullptr == this)
	{
		EngineGeneralDebug::GameDestroyer();
		return nullptr;
	}

	if (nullptr == object_)
	{
		EngineGeneralDebug::GameDestroyer();
		return nullptr;
	}

	if (nullptr == object_->GetScene())
	{
		EngineGeneralDebug::GameDestroyer();
		return nullptr;
	}

	// 최적화
	std::list<GameCollision*>& GroupList = object_->GetScene()->GetCollisionList(_otherIndex);
	std::list<GameCollision*>::iterator iter = GroupList.begin();
	
	// 특정 그룹인덱스를 가진 리스트만 하나 가져온다.
	if (true == GroupList.empty()) // 그 그룹 리스트가 비어 있다면...
	{
		return nullptr; // null 리턴
	}

	std::list<GameCollision*>::iterator StartIter = GroupList.begin();
	std::list<GameCollision*>::iterator EndIter = GroupList.end();
	for (; StartIter != EndIter; ++StartIter) // 이터레이터로 그 리스트를 타고 다니며...
	{
		if (true == CollisionCheck(*StartIter)) // CollisionCheck 에 걸리는 노드가 있다면,
		{
			return *StartIter; // 바로 반환!!
		}
	}

	return nullptr; // 충돌 안되면 nullptr, 충돌하면 그 충돌체를 반환할 것.
}



// 내가 xxx그룹에 속한 충돌체인데. 상대 xxx 그룹과 전부다 충돌해 보고 싶을때 쓰면 될 듯...
std::list<GameCollision*> GameCollision::CollisionGroupCheckMulti(int _otherIndex)
{
	std::list<GameCollision*> ReturnList;

	// 상대 그룹을 얻어와야 일단 뭔가를 시작할수 있다.
	if (nullptr == this)
	{
		EngineGeneralDebug::GameDestroyer();
		return ReturnList;
	}

	if (nullptr == object_)
	{
		EngineGeneralDebug::GameDestroyer();
		return ReturnList;
	}

	if (nullptr == object_->GetScene())
	{
		EngineGeneralDebug::GameDestroyer();
		return ReturnList;
	}

	// 최적화
	std::list<GameCollision*>& GroupList = object_->GetScene()->GetCollisionList(_otherIndex);

	if (true == GroupList.empty())
	{
		return ReturnList;
	}

	std::list<GameCollision*>::iterator StartIter = GroupList.begin();
	std::list<GameCollision*>::iterator EndIter = GroupList.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		if (true == CollisionCheck(*StartIter)) // 그 리스트의 가입자들을 하나씩 CollisionCheck 해 준다.
		{
			ReturnList.push_back(*StartIter); // 
		}
	}

	return ReturnList;
}

void GameCollision::StartDebugRender()
{
	ImageAsset* backBufferImage = AssetManager::GetAssetManager().GetNextBuffer();
	float4 objectPos = object_->GetCameraLinkedPos();

	if (false == isCameraRelated_)
	{
		objectPos = object_->GetPos();
	}

	float4 renderPos = objectPos + pivot_;

	switch (colType_)
	{
	case ColliderType::RECT:
	{
	Rectangle(backBufferImage->GetDC(), 
	renderPos.ix() - size_.ihx(),
	renderPos.iy() - size_.ihy(),
	renderPos.ix() + size_.ihx(),
	renderPos.iy() + size_.ihy());

break;
	}
	case ColliderType::MAX:
		break;
	default:
		break;
	}
}

Figure GameCollision::GetFigure()
{
	if (false == isCameraRelated_)
	{
		return { object_->GetPos() + pivot_, size_ };
	}

	return { object_->GetCameraLinkedPos() + pivot_, size_ };
}