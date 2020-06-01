#include "stdafx.h"
#include "SubWindow.h"
#include "gameNode.h"
#include "MapToolScene.h"

POINT SubWindow::ptMouse = POINT{ 0,0 };
CTRL SubWindow::_currentCTRL = CTRL_DRAW;

SubWindow::SubWindow()
{
	m_backBuffer = new image();
	m_backBuffer->init(SUBWINSIZEX, SUBWINSIZEY);
}

SubWindow::~SubWindow()
{
	SAFE_DELETE(m_backBuffer);
}

void SubWindow::init()
{
	CreateSubWindow();

	isActive = false;

	int tempX = 10;

	_btnDraw = CreateWindow("button", "Draw",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX, 0, 100, 20, hWnd, HMENU(CTRL_DRAW) , m_hInstance, NULL);
	_btnEraser = CreateWindow("button", "Eraser",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX, 30, 100, 20, hWnd, HMENU(CTRL_ERASER), m_hInstance, NULL);
	_btnInit = CreateWindow("button", "Init",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX, 60, 100, 20, hWnd, HMENU(CTRL_INIT), m_hInstance, NULL);
	_btnSave = CreateWindow("button", "Save",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 0, 100, 20, hWnd, HMENU(CTRL_SAVE), m_hInstance, NULL);
	_btnLoad = CreateWindow("button", "Load",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 30, 100, 20, hWnd, HMENU(CTRL_LOAD), m_hInstance, NULL);
	_btnN1 = CreateWindow("button", "MapTiles",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX, 135, 70, 25, hWnd, HMENU(CTRL_NUM1), m_hInstance, NULL);
	_btnN2 = CreateWindow("button", "Blocks",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 75, 135, 70, 25, hWnd, HMENU(CTRL_NUM2), m_hInstance, NULL);
	_btnN3 = CreateWindow("button", "Doors",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 150, 135, 70, 25, hWnd, HMENU(CTRL_NUM3), m_hInstance, NULL);
	_btnMain = CreateWindow("button", "MainMenu",
		//자식으로 생성하면 안쪽에 만들어짐
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 60, 100, 20, hWnd, HMENU(CTRL_MAIN), m_hInstance, NULL);

	clickFrame = { 0,0 };
	clickIndex = 0;

}

void SubWindow::release()
{
}

void SubWindow::update()
{
	if (currentScene != NULL)
	{
		currentScene->update();
	}
}

void SubWindow::render()
{
	HDC hdc = GetDC(hWnd);
	PatBlt(m_backBuffer->getMemDC(), 0, 0, SUBWINSIZEX, SUBWINSIZEY, WHITENESS);

	if (currentScene != NULL)
	{
		currentScene->render(m_backBuffer->getMemDC());
	}
	m_backBuffer->render(hdc);
	ReleaseDC(hWnd, hdc);
}

void SubWindow::SetScene(gameNode * scene)
{
	currentScene = scene;
	currentScene->init();
}

LRESULT SubWindow::WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:

		SUBWIN->SetIsActive(true);
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		break;

	case  WM_COMMAND:

		switch (LOWORD(wParam))
		{
		default:
			switch (LOWORD(wParam))
			{
			case CTRL_DRAW:
				_currentCTRL = (CTRL)(LOWORD(wParam));
				break;

			case CTRL_ERASER:
				_currentCTRL = (CTRL)(LOWORD(wParam));
				break;

			case CTRL_INIT:
				SUBWIN->GetIsoMap()->SetSubWindow();
				break;
			case CTRL_SAVE:
				if (SUBWIN->saveCount == 10)
				{
					SUBWIN->saveCount = 0;
				}
				SUBWIN->GetIsoMap()->Save(SUBWIN->saveCount);
				SUBWIN->saveCount += 1;
				break;
			case CTRL_LOAD:
				SUBWIN->GetIsoMap()->Load(RND->getInt(10));
				break;
			case CTRL_NUM1:
			case CTRL_NUM2:
			case CTRL_NUM3:
				SUBWIN->SetFrameIndex(LOWORD(wParam));
				break;
			case CTRL_MAIN:
				SCENEMANAGER->changeScene("MainMenu");
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SubWindow::CreateSubWindow()
{
	//로그 윈도우 생성
	int x, y, cx, cy;
	WNDCLASS wc;
	RECT rc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)SubWindow::WndLogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "sub";
	RegisterClass(&wc);
	//부모 윈도우 오른쪽에 위치 하자.
	RECT rcWin;
	GetWindowRect(m_hWnd, &rcWin);

	cx = SUBWINSIZEX;
	cy = SUBWINSIZEY;
	x = rcWin.right;
	y = rcWin.top;

	rc.left = 0;
	rc.top = 0;
	rc.right = cx;
	rc.bottom = cy;

	HWND hParenthWnd = NULL;
	HINSTANCE hInst = NULL;

	hParenthWnd = m_hWnd;
	hInst = GetModuleHandle(NULL);

	hWnd = CreateWindow("sub", "sub", WS_POPUP | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, cx, cy, hParenthWnd, NULL, hInst, NULL);

	AdjustWindowRect(&rc, WINSTYLE, FALSE);

	SetWindowPos(hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER);

	ShowWindow(hWnd, SW_SHOW);
}