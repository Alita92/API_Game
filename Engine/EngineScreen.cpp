#include "EngineScreen.h"
#include <EngineGeneralDebug.h>
#include <EngineGeneralTime.h>
#include "InputManager.h"
#include "ImageAsset.h"
#include "AssetManager.h"
#include <EngineGeneralSound.h>

EngineScreen* EngineScreen::Screen_ = new EngineScreen();

bool isScreenOn = true;

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) // ������â�� ������ ������ ���� Callback�Ǵ� �Լ�
{
	switch (_message) // switch �� ��ư�� ��������, ��� �ҷ�? �� ǥ���ϴ� CALLBACK ���
	{
	case WM_PAINT: // ���� �� �� �׷��� �� ���� ���� �� ȣ��... ���� ����������� ȭ���� ������� �׷��� �ϴ� �ѹ� ȣ��ȴ�.
	{
	PAINTSTRUCT ps;

	HDC hdc = BeginPaint(_hWnd, &ps);
	// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...

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

				// �̰� �Ƹ� �������� �׷��ִ¾��̴�.
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

	wcex.hIcon = nullptr; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PSEUDOWINAPI)); // ���α׷� �������� �ڵ鰪
	wcex.hCursor = nullptr; // LoadCursor(nullptr, IDC_ARROW); // Ŀ���� �ڵ鰪
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); // ȭ���� ����Ʈ ���� �ɼ�

	wcex.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_PSEUDOWINAPI); // ������ �޴����� id ��?
	wcex.lpszClassName = "GameScreen";  // ������ Ŭ������ id���� �����Ѵ�. �����츦 ���� �� �̰��� �����Ͽ� �����츦 ����
	// L�� ���� ����? wchar �� �����ڵ� ���ڿ��� ���� ������ ������(wchar ���� �ۼ��� L"���ڿ�" �� �ڵ带 �ۼ��Ѵ�.)
	wcex.hIconSm = nullptr; // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ���α׷��� ���� �������� �ڵ鰪

	RegisterClassExA(&wcex); // ������ Ŭ������ ��Ͻ�Ű�� �Լ�
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
				DispatchMessage(&msg); // ���⼭ WNDPROC�� ����ȴ�.
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
			
			_screenLoopFunc(); // ���ڷ� �� �Լ��� ����
		}
	}


}
