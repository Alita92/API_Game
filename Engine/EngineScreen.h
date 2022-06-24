#pragma once
#include <Windows.h>
#include <string>
#include <EngineGeneralMath.h>
#include <EngineGeneralNameBase.h>
#include <EngineGeneralMovie.h>

// 분류 : 엔진이며, 윈도우창(화면)을 정의
// 용도 :winapi의 윈도우 창을 제어하는 기능들에 대한 랩핑과 편의 함수의 제공
// 설명 :싱글톤 패턴의 객체화를 사용한다.
class ImageAsset;
class EngineScreen : public EngineGeneralNameBase
{
private:	// 싱글톤 객체화 변수
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
	EngineScreen(); // default constructer 디폴트 생성자
	~EngineScreen(); // default destructer 디폴트 소멸자

public:		// delete constructer
	EngineScreen(const EngineScreen& _other) = delete; // default Copy constructer 디폴트 복사생성자
	EngineScreen(EngineScreen&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	EngineScreen& operator=(const EngineScreen& _other) = delete; // default Copy operator 디폴트 대입 연산자
	EngineScreen& operator=(const EngineScreen&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	void SetSizePos(const float4& _size, const float4& _pos);
	void ScreenLoop(void(*_screenLoopFunc)());
};