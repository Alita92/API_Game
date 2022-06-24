#pragma once

// 분류 :
// 용도 :
// 설명 :
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
	EngineGeneralObjectBase(); // default constructer 디폴트 생성자
	~EngineGeneralObjectBase(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineGeneralObjectBase(const EngineGeneralObjectBase& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EngineGeneralObjectBase(EngineGeneralObjectBase&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineGeneralObjectBase& operator=(const EngineGeneralObjectBase& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineGeneralObjectBase& operator=(const EngineGeneralObjectBase&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
};

