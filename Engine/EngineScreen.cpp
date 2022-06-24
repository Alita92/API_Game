#include "EngineScreen.h"
#include <EngineGeneralDebug.h>
#include <EngineGeneralTime.h>
#include "InputManager.h"
#include "ImageAsset.h"
#include "AssetManager.h"
#include <EngineGeneralSound.h>

EngineScreen* EngineScreen::Screen_ = new EngineScreen();

bool isScreenOn = true;

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) // 윈도우창에 직접적 조작이 들어갈때 Callback되는 함수
{
	switch (_message) // switch 로 버튼이 눌러졌어, 어떻게 할래? 를 표현하는 CALLBACK 방식
	{
	case WM_PAINT: // 뭔가 한 번 그려야 할 일이 생길 때 호출... 최초 만들어질때도 화면을 흰색으로 그려야 하니 한번 호출된다.
	{
	PAINTSTRUCT ps;

	HDC hdc = BeginPaint(_hWnd, &ps);
	// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

	if (nullptr != EngineScreen::GetScreen().GetMoviePlayer())
	{
		if (true == EngineScreen::GetScreen().GetMoviePlayer()->IsPlay())
		{
			if (EngineScreen::GetScreen().GetMoviePlayer()->State() != STATE_NO_GRAPH &&
				EngineScreen::GetScreen().GetMoviePlayer()->HasVideo())
			{
				RECT rc;
				GetClientRect(EngineScreen::GetScreen().GetWindowHWND(), &rc);

				EngineScreen::GetScreen().GetMoviePlayer()->UpdateVideoWindow(&rc);

				// 이게 아마 동영상을 그려주는애이다.
				// The player has video, so ask the player to repaint. 
				EngineScreen::GetScreen().GetMoviePlayer()->Repaint(EngineScreen::GetScreen().GetWindowDC());
			}
			else
			{
				FillRect(EngineScreen::GetScreen().GetWindowDC(), &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			}
		}
	}

	EndPaint(_hWnd, &ps);
	break;
	}
	case WM_SIZE:
	{
		if (nullptr != EngineScreen::GetScreen().GetMoviePlayer())
		{
			if (true == EngineScreen::GetScreen().GetMoviePlayer()->IsPlay())
			{
				RECT rc;
				GetClientRect(EngineScreen::GetScreen().GetWindowHWND(), &rc);
				EngineScreen::GetScreen().GetMoviePlayer()->UpdateVideoWindow(&rc);
			}
		}

		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		break;
	}
	case WM_DESTROY:
	{
		isScreenOn = false;
		break;
	}
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

// constructer destructer
EngineScreen::EngineScreen()
	: className_(""), screenTitle_(""), screenHandle_(nullptr), hInstance_(nullptr), devicecontext_(nullptr)
{
}

EngineScreen::~EngineScreen()
{
	if (nullptr != moviePlayer_)
	{
		delete moviePlayer_;
	}
}

EngineScreen::EngineScreen(EngineScreen&& _other) noexcept
	: className_(""), screenTitle_(""), screenHandle_(nullptr), hInstance_(nullptr), devicecontext_(nullptr)
{
}

void EngineScreen::CreateScreenClass(HINSTANCE _hInstance, std::string _className)
{
	if (nullptr == _hInstance)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}
	if ("" == _className)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	hInstance_ = _hInstance;
	className_ = _className;

	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; 
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;

	wcex.hIcon = nullptr; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PSEUDOWINAPI)); // 프로그램 아이콘의 핸들값
	wcex.hCursor = nullptr; // LoadCursor(nullptr, IDC_ARROW); // 커서의 핸들값
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); // 화면의 디폴트 색깔 옵션

	wcex.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_PSEUDOWINAPI); // 윈도우 메뉴바의 id 값?
	wcex.lpszClassName = "GameScreen";  // 윈도우 클래스의 id값을 세팅한다. 윈도우를 생성 시 이값을 참조하여 윈도우를 생성
	// L이 붙은 이유? wchar 형 유니코드 문자열의 값을 가지기 때문에(wchar 형을 작성시 L"문자열" 로 코드를 작성한다.)
	wcex.hIconSm = nullptr; // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // 프로그램의 소형 아이콘의 핸들값

	RegisterClassExA(&wcex); // 윈도우 클래스를 등록시키는 함수
}


void EngineScreen::CreateScreen(const std::string _screenTitle, const float4& _size, const float4& _pos)
{
	if (nullptr == hInstance_)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}
	if ("" == className_)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	screenTitle_ = _screenTitle;


	screenHandle_ = CreateWindowA(className_.c_str(), screenTitle_.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance_, nullptr);

	if (nullptr == screenHandle_)
	{
		EngineGeneralDebug::GameDestroyer();
		return;
	}

	ShowWindow(screenHandle_, SW_SHOW);
	SetSizePos(_size, _pos);
	UpdateWindow(screenHandle_);

	HDC DCBuffer = ::GetDC(screenHandle_);
	AssetManager::GetAssetManager().InitializeScreenImage(DCBuffer);

	moviePlayer_ = new EngineGeneralMovie(screenHandle_);
	return;
}

void EngineScreen::SetSizePos(const float4& _size, const float4& _pos)
{
	size_ = _size;
	pos_ = _pos;

	RECT Rc = { 0, 0, _size.ix(), _size.iy() };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(screenHandle_, nullptr, _pos.ix(), _pos.iy(), Rc.right - Rc.left, Rc.bottom - Rc.top, 0);
}

void EngineScreen::ScreenLoop(void(*_screenLoopFunc)())
{
	MSG msg;

	while (isScreenOn)
	{
		if (0 != PeekMessage(&msg, screenHandle_, 0, 0, PM_REMOVE)) 
		{   
			if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg); // 여기서 WNDPROC이 실행된다.
			}
		}
		else
		{
			EngineGeneralTime::GetEngineGeneralTime().CheckTime();
			InputManager::GetInputManager().InputUpdate();
			EngineGeneralSound::GetEngineGeneralSound().SoundUpdate();
			
			if (nullptr == _screenLoopFunc)
			{
				EngineGeneralDebug::GameDestroyer();
				return;
			}
			
			_screenLoopFunc(); // 인자로 쓴 함수의 실행
		}
	}


}
