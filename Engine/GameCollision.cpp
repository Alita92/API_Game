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
bool GameCollision::CollisionCheck(GameCollision* _other) // ���� This �� Collision �� Ÿ collision �浹�� üũ�ϴ� �Լ�
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

	// �� ���ӳ������� RectToRect �� ������
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
	// ��� �׷��� ���;� �ϴ� ������ �����Ҽ� �ִ�.
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

	// ����ȭ
	std::list<GameCollision*>& GroupList = object_->GetScene()->GetCollisionList(_otherIndex);
	std::list<GameCollision*>::iterator iter = GroupList.begin();
	
	// Ư�� �׷��ε����� ���� ����Ʈ�� �ϳ� �����´�.
	if (true == GroupList.empty()) // �� �׷� ����Ʈ�� ��� �ִٸ�...
	{
		return nullptr; // null ����
	}

	std::list<GameCollision*>::iterator StartIter = GroupList.begin();
	std::list<GameCollision*>::iterator EndIter = GroupList.end();
	for (; StartIter != EndIter; ++StartIter) // ���ͷ����ͷ� �� ����Ʈ�� Ÿ�� �ٴϸ�...
	{
		if (true == CollisionCheck(*StartIter)) // CollisionCheck �� �ɸ��� ��尡 �ִٸ�,
		{
			return *StartIter; // �ٷ� ��ȯ!!
		}
	}

	return nullptr; // �浹 �ȵǸ� nullptr, �浹�ϸ� �� �浹ü�� ��ȯ�� ��.
}



// ���� xxx�׷쿡 ���� �浹ü�ε�. ��� xxx �׷�� ���δ� �浹�� ���� ������ ���� �� ��...
std::list<GameCollision*> GameCollision::CollisionGroupCheckMulti(int _otherIndex)
{
	std::list<GameCollision*> ReturnList;

	// ��� �׷��� ���;� �ϴ� ������ �����Ҽ� �ִ�.
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

	// ����ȭ
	std::list<GameCollision*>& GroupList = object_->GetScene()->GetCollisionList(_otherIndex);

	if (true == GroupList.empty())
	{
		return ReturnList;
	}

	std::list<GameCollision*>::iterator StartIter = GroupList.begin();
	std::list<GameCollision*>::iterator EndIter = GroupList.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		if (true == CollisionCheck(*StartIter)) // �� ����Ʈ�� �����ڵ��� �ϳ��� CollisionCheck �� �ش�.
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