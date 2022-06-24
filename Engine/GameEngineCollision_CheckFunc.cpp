#include "GameCollision.h"
#include <EngineGeneralDebug.h>
#include "ImageAsset.h"

// ��� �Լ� �����Ϳ� �Լ���


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
	// �������� ������ ����ǥ�� �̹����� �浹�̶� �̷��� �����Ͻ�.
	// 2�� ������ �簢�� & �簢�� �浹 �Լ� ����°� Ȯ��.
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
