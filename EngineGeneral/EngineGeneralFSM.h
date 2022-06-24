#pragma once
#include "EngineGeneralNameBase.h"
#include "EngineGeneralDebug.h"
#include <map>
#include <string>
// 분류 :
// 용도 :
// 설명 :


// 이 FSM 은 몬스터를 / 플레이어를 관리해 줄 거야
// 이걸 할려면 가장 효율적인 방법은 템플릿을 사용하는 것.
//template<typename T>
//class EngineGeneralFSM
//{
//private:	// member Var
//	class FSMState : public EngineGeneralNameBase
//	{
//		friend EngineGeneralFSM;
//		FSMState* objectPtr_; // 멤버 함수 포인터의 객체화된 클래스 포인터
//		void (T::* FSMStateStart)(); // 멤버함수 포인터 :: 내가 관리해주고 싶은 class(Player, Monster) 의 멤버 함수를 실행시킬 수 있다.
//		void (T::* FSMStateUpdate)();
//
//	public:
//		void CallFSMStateStart()
//		{
//			if (nullptr == objectPtr_) // 객체가 null 이면 터뜨리기
//			{
//				EngineGeneralDebug::GameDestroyer();
//				return;
//			}
//			(objectPtr_->*FSMStateStart)(); // 멤버 함수 포인터 StateStart() 로 장전
//		}
//		void CallFSMStateUpdate()
//		{
//			if (nullptr == objectPtr_) // 객체가 null 이면 터뜨리기
//			{
//				EngineGeneralDebug::GameDestroyer();
//				return;
//			}
//			(objectPtr_->*FSMStateUpdate)();  // 멤버 함수 포인터 StateUpdate() 로 장전
//
//		}
//	public:
//		void UpdateFSMState() {};
//
//	};
//
//public:
//	EngineGeneralFSM()
//		: curFSMState_(nullptr)
//	{
//	}
//	~EngineGeneralFSM()
//	{
//
//	}
//
//public:		// delete constructer
//	EngineGeneralFSM(const EngineGeneralFSM& _other) = delete; // default Copy constructer 디폴트 복사생성자
//	EngineGeneralFSM(EngineGeneralFSM&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
//
//public:		//delete operator
//	EngineGeneralFSM& operator=(const EngineGeneralFSM& _other) = delete; // default Copy operator 디폴트 대입 연산자
//	EngineGeneralFSM& operator=(const EngineGeneralFSM&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
//
//private:
//	std::map < std::string, FSMState*> mapFSMState_;
//	FSMState* curFSMState_;
//
//private:
//	FSMState* FindFSMState(const std::string& _name);
//
//public:		//member Func
//	FSMState* FindFSMState(const std::string& _name)
//	{
//		std::map<std::string, FSMState*>::iterator findIter = mapFSMState_.find(_name);
//
//		if (findIter == mapFSMState_.end())
//		{
//			return nullptr;
//		}
//
//		return findIter->second;
//	}
//
//
//	void ChangeFSMState(const std::string& _name)
//	{
//		curFSMState_ = FindFSMState(_name);
//
//		if (nullptr == curFSMState_)
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return;
//		}
//		curFSMState_->CallFSMStateStart();
//	}
//
//	bool IsCurFSMStateName(const std::string& _name)
//	{
//		if (nullptr == curFSMState_)
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return false;
//		}
//
//		if (curFSMState_->GetName() == _name)
//		{
//			return true;
//		}
//
//		return false;
//	}
//
//
//	void Update()
//	{
//		if (nullptr == curFSMState_) // State 가 존재하지 않는 상황은 허락하지 않는다.
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return;
//		}
//
//		curFSMState_->CallFSMStateUpdate();
//	}
//
//
//	void CreateFSMState(const std::string& _name)
//	{
//		if (nullptr != FindFSMState(_name))
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return;
//		}
//
//		ImageAsset* imageAssetPtr = AssetManager::GetAssetManager().FindImportedImage(_imageName); // 임포트된 이미지가 없다?
//
//		if (nullptr == imageAssetPtr)
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return;
//		}
//
//		if (false == imageAssetPtr->isCut()) // 잘려있지 않다면 파괴
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return;
//		}
//
//		SpriteAnimation* newAnimation = new SpriteAnimation();
//
//		newAnimation->SetName(_name);
//		newAnimation->startFrame_ = _startFrameOrder; // ~ 번째부터의 스프라이트부터
//		newAnimation->endFrame_ = _endFrameOrder; // ~ 번째까지의 스프라이트를 사용할지
//		newAnimation->interTime_ = _delayTime; // 기본 딜레이타임 설정
//		newAnimation->linkedImage_ = imageAssetPtr;
//		newAnimation->curFrame_ = _startFrameOrder;
//		newAnimation->curInterTime_ = _delayTime;
//		newAnimation->pivotEnum_ = _pivotEnum;
//		newAnimation->linkedRenderer_ = this;
//		newAnimation->isLoop_ = _isLoop;
//
//		mapAnimation_.insert(std::map<std::string, SpriteAnimation*>::value_type(_name, newAnimation));
//		// 맵구조 등록
//	}
//};

