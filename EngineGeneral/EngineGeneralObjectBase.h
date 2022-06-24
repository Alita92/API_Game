#pragma once

// �з� :
// �뵵 :
// ���� :
class EngineGeneralObjectBase
{
private:	// member Var
	bool isDeath_;
	bool isUpdate_;
	bool isDebug_;

	EngineGeneralObjectBase* parent_;

public:
	void SetParent(EngineGeneralObjectBase* _parent)
	{
		parent_ = _parent;
	}

public:
	bool IsDeath()
	{
		if (nullptr == parent_)
		{
			return isDeath_;
		}

		return true == isDeath_ || parent_->isDeath_;
	}

	bool IsOn()
	{
		if (nullptr == parent_)
		{
			return isUpdate_;
		}

		return true == isUpdate_ || parent_->isUpdate_;
	}


	void Death()
	{
		isDeath_ = true;
	}

	void On()
	{
		isUpdate_ = true;
	}

	void Off()
	{
		isUpdate_ = false;
	}

	void DebugOn()
	{
		isDebug_ = true;
	}

	void DebugOff()
	{
		isDebug_ = false;
	}

	bool IsDebugOn()
	{
		return isDebug_;
	}


public:		
	EngineGeneralObjectBase(); // default constructer ����Ʈ ������
	~EngineGeneralObjectBase(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineGeneralObjectBase(const EngineGeneralObjectBase& _other) = delete; // default Copy constructer ����Ʈ ���������
	EngineGeneralObjectBase(EngineGeneralObjectBase&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineGeneralObjectBase& operator=(const EngineGeneralObjectBase& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineGeneralObjectBase& operator=(const EngineGeneralObjectBase&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

