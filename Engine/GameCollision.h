#pragma once
#include "EngineENUM.h"
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <list>
#include <Windows.h>

class Object;
class SceneManager;
class ImageAsset;
class GameCollision : public EngineGeneralNameBase
{
private:
	static bool (*arrFunc_[static_cast<int>(ColliderType::MAX)][static_cast<int>(ColliderType::MAX)])(GameCollision* _PointCol, GameCollision* _ImageCol);
	// ����Լ� ������; ������� �迭�� 1���� 2���� ColliderType �� ���� 
	static void ColCheckFuncInitialize();

public:
	static bool PointToImage(GameCollision* _PointCol, GameCollision* _ImageCol);
	static bool ImageToPoint(GameCollision* _ImageCol, GameCollision* _PointCol);
	static bool RectToRect(GameCollision* _PointCol, GameCollision* _ImageCol);
private:

	friend SceneManager;
	friend Object;
private:	
	Object* object_; // 
	float4 pivot_;
	float4 size_;

	ColliderType colType_;
	int groupIndex_; // �浹 ����Ʈ�� ������ ��ȣ. (�������� 0)
	//	1P 1, 2P 2, �ٴ� 3 ���� �ϴ� ������ ��.

	ImageAsset* imageptr_;
	DWORD CheckColor;

	int collisionCount_;

private:
	float4 GetCollisionPos();


public:
	void SetImportedImage(std::string _ImageName);


public:
	void SetCollisionCount(int _Count)
	{
		collisionCount_ = _Count;
	}

	void SetColorCheck(DWORD _Color)
	{
		CheckColor = _Color;
	}

	void SetGroupIndex(int _Index)
	{
		groupIndex_ = _Index;
	}

	void SetColType(ColliderType _colType)
	{
		colType_ = _colType;
	}

	void SetPivot(float4 _pivot)
	{
		pivot_ = _pivot;
	}

	void SetSize(float4 _size)
	{
		size_ = _size;
	}

public:
	int GetTypeToIndex()
	{
		return static_cast<int>(colType_);
	}

	int GetGroupIndex()
	{
		return groupIndex_;
	}

	DWORD GetColorCheck()
	{
		return CheckColor;
	}

	bool isCameraRelated_;

public:
	void SetObject(Object* _parent)
	{
		object_ = _parent;
	}

	void SetCameraEffectOn()
	{
		isCameraRelated_ = true;
	}

	void SetCameraEffectOff()
	{
		isCameraRelated_ = false;
	}

private:		
	GameCollision(); // default constructer ����Ʈ ������
	~GameCollision(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	GameCollision(const GameCollision& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameCollision(GameCollision&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	GameCollision& operator=(const GameCollision& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameCollision& operator=(const GameCollision&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void StartDebugRender();

	bool CollisionCheck(GameCollision* _other);


	template<typename COLGROUP>
	GameCollision* CollisionGroupCheckSingle(COLGROUP _otherGroup) // �׷���� �ٸ� �浹ü�� '�� ��' �浹�� ���� üũ�Ѵ�.
	{
		return CollisionGroupCheckSingle(static_cast<int>(_otherGroup));
	}

	GameCollision* CollisionGroupCheckSingle(int _otherIndex); // �׷� ��ȣ�� �ٸ� �ٸ� �浹ü�� '�� ��' �浹�� ���� üũ�Ѵ�.
	std::list<GameCollision*> CollisionGroupCheckMulti(int _otherIndex); // ���ôٹ��� ������ �浹 üũ

	Figure GetFigure();

};

