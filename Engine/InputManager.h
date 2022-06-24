#pragma once
#include <map>
#include <list>
#include <EngineGeneralNameBase.h>

// 분류 : 
// 용도 : 게임의 모든 입력 키의 생성과 관리를 담당합니다.
// 설명 :
class EngineScreen;
class InputManager
{
	friend EngineScreen;
private:	// member Var
	class InputKey : public EngineGeneralNameBase
	{
		friend InputManager;

	private:
		bool buttonDown_; // 최초 1회 눌렸을때 true 이번 프레임에 한번 체크가 됩니다.
		bool buttonPress_; // 지속적으로 누를때 true
		bool buttonUp_; // 이번 프레임에 1번만 땠다
		bool buttonFree_; // 지금 안누르고 있다.
		int buttonKey_; // 내가 누르려는 키의 인덱스 혹은 타입
		bool isMoveKey_;

	public:
		void InputKeyUpdate();

	private:
		InputKey();
		~InputKey();
	};

private:
	static InputManager* inputManager_;

public:
	static InputManager& GetInputManager()
	{
		return *inputManager_;
	}


	static void DestroyMemory()
	{
		if (nullptr != inputManager_)
		{
			delete inputManager_;
			inputManager_ = nullptr;
		}
	}

private:
	std::map<std::string, InputKey*>mapKeys_;

public:		
	InputManager(); // default constructer 디폴트 생성자
	~InputManager(); // default destructer 디폴트 소멸자

public:		// delete constructer
	InputManager(const InputManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	InputManager(InputManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	InputManager& operator=(const InputManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	InputManager& operator=(const InputManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	bool CreateInputKey(const std::string _name, int _key, bool _isMoveKey = false);

	bool IsButtonDown(const std::string _name);
	bool IsButtonUp(const std::string _name);
	bool IsButtonPress(const std::string _name);
	bool IsButtonFree(const std::string _name);
	bool IsButtonMove(const std::string _name);


	// 이 키를 만든적이 있어?
	bool IsButtonKey(const std::string _name);


private:
	InputKey* FindButtonKey(const std::string _name);
	void InputUpdate();




};

