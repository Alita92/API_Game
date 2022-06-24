#pragma once
#include <map>
#include <list>
#include <EngineGeneralNameBase.h>

// �з� : 
// �뵵 : ������ ��� �Է� Ű�� ������ ������ ����մϴ�.
// ���� :
class EngineScreen;
class InputManager
{
	friend EngineScreen;
private:	// member Var
	class InputKey : public EngineGeneralNameBase
	{
		friend InputManager;

	private:
		bool buttonDown_; // ���� 1ȸ �������� true �̹� �����ӿ� �ѹ� üũ�� �˴ϴ�.
		bool buttonPress_; // ���������� ������ true
		bool buttonUp_; // �̹� �����ӿ� 1���� ����
		bool buttonFree_; // ���� �ȴ����� �ִ�.
		int buttonKey_; // ���� �������� Ű�� �ε��� Ȥ�� Ÿ��
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
	InputManager(); // default constructer ����Ʈ ������
	~InputManager(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	InputManager(const InputManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	InputManager(InputManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	InputManager& operator=(const InputManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	InputManager& operator=(const InputManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	bool CreateInputKey(const std::string _name, int _key, bool _isMoveKey = false);

	bool IsButtonDown(const std::string _name);
	bool IsButtonUp(const std::string _name);
	bool IsButtonPress(const std::string _name);
	bool IsButtonFree(const std::string _name);
	bool IsButtonMove(const std::string _name);


	// �� Ű�� �������� �־�?
	bool IsButtonKey(const std::string _name);


private:
	InputKey* FindButtonKey(const std::string _name);
	void InputUpdate();




};

