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
	// 멤버함수 포인터; 만들어진 배열내 1차와 2차의 ColliderType 를 비교해 
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
	int groupIndex_; // 충돌 리스트를 구분할 번호. (무지정은 0)
	//	1P 1, 2P 2, 바닥 3 으로 일단 설정할 것.

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
	GameCollision(); // default constructer 디폴트 생성자
	~GameCollision(); // default destructer 디폴트 소멸자

public:		// delete constructer
	GameCollision(const GameCollision& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameCollision(GameCollision&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	GameCollision& operator=(const GameCollision& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameCollision& operator=(const GameCollision&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	void StartDebugRender();

	bool CollisionCheck(GameCollision* _other);


	template<typename COLGROUP>
	GameCollision* CollisionGroupCheckSingle(COLGROUP _otherGroup) // 그룹명이 다른 충돌체와 '한 번' 충돌한 것을 체크한다.
	{
		return CollisionGroupCheckSingle(static_cast<int>(_otherGroup));
	}

	GameCollision* CollisionGroupCheckSingle(int _otherIndex); // 그룹 번호가 다른 다른 충돌체와 '한 번' 충돌한 것을 체크한다.
	std::list<GameCollision*> CollisionGroupCheckMulti(int _otherIndex); // 동시다발적 여러번 충돌 체크

	Figure GetFigure();

};

