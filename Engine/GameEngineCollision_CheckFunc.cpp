#include "GameCollision.h"
#include <EngineGeneralDebug.h>
#include "ImageAsset.h"

// 멤버 함수 포인터용 함수들


bool (*GameCollision::arrFunc_[static_cast<int>(ColliderType::MAX)][static_cast<int>(ColliderType::MAX)])(GameCollision* _left, GameCollision* _right) = { nullptr };

void GameCollision::ColCheckFuncInitialize()
{
	for (size_t y = 0; y < static_cast<int>(ColliderType::MAX); y++)
	{
		for (size_t x = 0; x < static_cast<int>(ColliderType::MAX); x++)
		{
			arrFunc_[y][x] = nullptr;
		}
	}

	arrFunc_[static_cast<int>(ColliderType::RECT)][static_cast<int>(ColliderType::RECT)] = &GameCollision::RectToRect;
	// 선생님의 예제는 점좌표와 이미지의 충돌이라 이렇게 설정하심.
	// 2일 수업때 사각형 & 사각형 충돌 함수 만드는거 확인.
}

bool GameCollision::PointToImage(GameCollision* _PointCol, GameCollision* _ImageCol)
{
	float4 Pos = _PointCol->GetCollisionPos();

	if (nullptr == _ImageCol->imageptr_)
	{
		EngineGeneralDebug::GameDestroyer();
		return false;
	}

	DWORD Color = _ImageCol->imageptr_->GetColor4Byte(Pos.ix(), Pos.iy());

	return _PointCol->GetColorCheck() == Color;
}

bool GameCollision::ImageToPoint(GameCollision* _ImageCol, GameCollision* _PointCol)
{
	return false;
}


bool GameCollision::RectToRect(GameCollision* _left, GameCollision* _right)
{
	Figure Left = _left->GetFigure();
	Figure Right = _right->GetFigure();

	if (Left.iRight() < Right.iLeft())
	{
		return false;
	}

	if (Left.iLeft() > Right.iRight())
	{
		return false;
	}

	if (Left.iTop() > Right.iBot())
	{
		return false;
	}

	if (Left.iBot() < Right.iTop())
	{
		return false;
	}

	return true;
}
