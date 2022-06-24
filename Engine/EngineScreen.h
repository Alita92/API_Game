#pragma once
#include <Windows.h>
#include <string>
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <EngineGeneralMovie.h>

// �з� : �����̸�, ������â(ȭ��)�� ����
// �뵵 :winapi�� ������ â�� �����ϴ� ��ɵ鿡 ���� ���ΰ� ���� �Լ��� ����
// ���� :�̱��� ������ ��üȭ�� ����Ѵ�.
class ImageAsset;
class EngineScreen : public EngineGeneralNameBase
{
private:	// �̱��� ��üȭ ����
	static EngineScreen* Screen_;

public:
	static EngineScreen& GetScreen()
	{
		return *Screen_;
	}

	static void DestroyMemory()
	{
		if (nullptr != Screen_)
		{
			delete Screen_;
			Screen_ = nullptr;
		}
	}

public:
	std::string className_;
	std::string screenTitle_;
	HINSTANCE hInstance_;
	HWND screenHandle_;
	HDC devicecontext_;

public:
	float4 size_;
	float4 pos_;

	EngineGeneralMovie* moviePlayer_;

public:
	EngineGeneralMovie* GetMoviePlayer()
	{
		return moviePlayer_;
	}

	HWND  GetWindowHWND()
	{
		return screenHandle_;
	}

	HDC  GetWindowDC()
	{
		return devicecontext_;
	}


public:
	float4 GetSize()
	{
		return size_;
	}

	float4 GetPos()
	{
		return pos_;
	}


public:
	void CreateScreenClass(HINSTANCE _hInstance, std::string _className);
	void CreateScreen(const std::string _screenTitle, const float4& _size, const float4& _pos);

private:		
	EngineScreen(); // default constructer ����Ʈ ������
	~EngineScreen(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	EngineScreen(const EngineScreen& _other) = delete; // default Copy constructer ����Ʈ ���������
	EngineScreen(EngineScreen&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	EngineScreen& operator=(const EngineScreen& _other) = delete; // default Copy operator ����Ʈ ���� ������
	EngineScreen& operator=(const EngineScreen&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	void SetSizePos(const float4& _size, const float4& _pos);
	void ScreenLoop(void(*_screenLoopFunc)());
};