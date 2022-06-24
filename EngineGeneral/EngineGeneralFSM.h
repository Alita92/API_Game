#pragma once
#include "EngineGeneralNameBase.h"
#include "EngineGeneralDebug.h"
#include <map>
#include <string>
// �з� :
// �뵵 :
// ���� :


// �� FSM �� ���͸� / �÷��̾ ������ �� �ž�
// �̰� �ҷ��� ���� ȿ������ ����� ���ø��� ����ϴ� ��.
//template<typename T>
//class EngineGeneralFSM
//{
//private:	// member Var
//	class FSMState : public EngineGeneralNameBase
//	{
//		friend EngineGeneralFSM;
//		FSMState* objectPtr_; // ��� �Լ� �������� ��üȭ�� Ŭ���� ������
//		void (T::* FSMStateStart)(); // ����Լ� ������ :: ���� �������ְ� ���� class(Player, Monster) �� ��� �Լ��� �����ų �� �ִ�.
//		void (T::* FSMStateUpdate)();
//
//	public:
//		void CallFSMStateStart()
//		{
//			if (nullptr == objectPtr_) // ��ü�� null �̸� �Ͷ߸���
//			{
//				EngineGeneralDebug::GameDestroyer();
//				return;
//			}
//			(objectPtr_->*FSMStateStart)(); // ��� �Լ� ������ StateStart() �� ����
//		}
//		void CallFSMStateUpdate()
//		{
//			if (nullptr == objectPtr_) // ��ü�� null �̸� �Ͷ߸���
//			{
//				EngineGeneralDebug::GameDestroyer();
//				return;
//			}
//			(objectPtr_->*FSMStateUpdate)();  // ��� �Լ� ������ StateUpdate() �� ����
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
//	EngineGeneralFSM(const EngineGeneralFSM& _other) = delete; // default Copy constructer ����Ʈ ���������
//	EngineGeneralFSM(EngineGeneralFSM&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
//
//public:		//delete operator
//	EngineGeneralFSM& operator=(const EngineGeneralFSM& _other) = delete; // default Copy operator ����Ʈ ���� ������
//	EngineGeneralFSM& operator=(const EngineGeneralFSM&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
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
//		if (nullptr == curFSMState_) // State �� �������� �ʴ� ��Ȳ�� ������� �ʴ´�.
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
//		ImageAsset* imageAssetPtr = AssetManager::GetAssetManager().FindImportedImage(_imageName); // ����Ʈ�� �̹����� ����?
//
//		if (nullptr == imageAssetPtr)
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return;
//		}
//
//		if (false == imageAssetPtr->isCut()) // �߷����� �ʴٸ� �ı�
//		{
//			EngineGeneralDebug::GameDestroyer();
//			return;
//		}
//
//		SpriteAnimation* newAnimation = new SpriteAnimation();
//
//		newAnimation->SetName(_name);
//		newAnimation->startFrame_ = _startFrameOrder; // ~ ��°������ ��������Ʈ����
//		newAnimation->endFrame_ = _endFrameOrder; // ~ ��°������ ��������Ʈ�� �������
//		newAnimation->interTime_ = _delayTime; // �⺻ ������Ÿ�� ����
//		newAnimation->linkedImage_ = imageAssetPtr;
//		newAnimation->curFrame_ = _startFrameOrder;
//		newAnimation->curInterTime_ = _delayTime;
//		newAnimation->pivotEnum_ = _pivotEnum;
//		newAnimation->linkedRenderer_ = this;
//		newAnimation->isLoop_ = _isLoop;
//
//		mapAnimation_.insert(std::map<std::string, SpriteAnimation*>::value_type(_name, newAnimation));
//		// �ʱ��� ���
//	}
//};

