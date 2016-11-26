
#include "Role.h"
#include "Map.h"
#include "FPSTools.h"
#include "Timer.h"
#include "Menu.h"

#define WND_CLASS_NAME "MAIN"
#define WND_NAME "SUPER MYRIO"
#define SAFE_RELEASE(p) { if (p) { p->Release(); p = NULL; } }

int g_WndWidth = 800;
int g_WndHeight = 600;
bool g_IsWindowed = true;
LPDIRECT3DDEVICE9 g_pD3DD = NULL;
LPDIRECTINPUTDEVICE8 g_pKeyBoardDevice = NULL;
LPD3DXSPRITE g_pD3DXSprite = NULL;
LPDIRECTSOUND8 g_pDirectSound = NULL;
FPSTools* g_pFPS = NULL;
Timer* g_pTimer = NULL;
Map* g_Map = NULL;
Menu* g_Menu = NULL;
HWND g_Hwnd;
char g_KeyBuffer[256];

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT Direct3D_Init(HWND hWnd);
HRESULT Game_Init();
void MapInit();
void Game_Render();
void Game_Release();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nCmdShow)
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = WND_CLASS_NAME;
	wndClass.style = CS_CLASSDC;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hIcon = (HICON)LoadImage(hInstance, "ico/1.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hIconSm = (HICON)LoadImage(hInstance, "ico/2.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(NULL, "Class Init Failed!", "Alert", MB_OK);
		return 0;
	}

	HWND hWnd = CreateWindow(WND_CLASS_NAME, WND_NAME,
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT, 
		g_WndWidth, g_WndHeight, NULL, NULL, hInstance, NULL);

	if (NULL == hWnd)
	{
		MessageBox(NULL, "Window Init Failed!", "Alert", MB_OK);
		return 0;
	}
	g_Hwnd = hWnd;
	if (S_OK != Direct3D_Init(hWnd))
	{
		MessageBox(NULL, "Direct3D_Init Failed!", "Alert", MB_OK);
		return 0;
	}

	if (S_OK != Game_Init())
	{
		MessageBox(NULL, "Game_Init Failed!", "Alert", MB_OK);
		return 0;
	}

	UpdateWindow(hWnd);
	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (g_pTimer->during())
			Game_Render();
	}

	UnregisterClass(WND_CLASS_NAME, hInstance);
	Game_Release();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		Game_Render();
		ValidateRect(hWnd, NULL);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

HRESULT Direct3D_Init(HWND hWnd)
{
	//d3ddevice init
	LPDIRECT3D9 pD3D = NULL;
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == pD3D)
	{
		MessageBox(NULL, "D3D Init Failed!", "Alert", MB_OK);
		return E_FAIL;
	}

	D3DCAPS9 caps;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		MessageBox(NULL, "Caps Init Failed!", "Alert", MB_OK);
		return E_FAIL;
	}
	int behaviorFlags;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DDISPLAYMODE displayMode;
	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

	D3DPRESENT_PARAMETERS ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.BackBufferCount = 1;
	ddsd.hDeviceWindow = hWnd;
	ddsd.BackBufferFormat = displayMode.Format;
	ddsd.SwapEffect = D3DSWAPEFFECT_DISCARD;
	ddsd.EnableAutoDepthStencil = true;
	ddsd.AutoDepthStencilFormat = D3DFMT_D24S8;
	ddsd.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	ddsd.MultiSampleQuality = 0;
	ddsd.MultiSampleType = D3DMULTISAMPLE_NONE;
	ddsd.Flags = 0;
	ddsd.Windowed = g_IsWindowed;
	if (g_IsWindowed)
	{
		ddsd.FullScreen_RefreshRateInHz = 0;
	}
	else
	{
		ddsd.FullScreen_RefreshRateInHz = displayMode.RefreshRate;
		ddsd.BackBufferHeight = g_WndHeight;
		ddsd.BackBufferWidth = g_WndWidth;
	}

	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, behaviorFlags, &ddsd, &g_pD3DD)))
	{
		MessageBox(NULL, "D3DDevice Init Failed!", "Alert", MB_OK);
		return E_FAIL;
	}

	SAFE_RELEASE(pD3D);

	//d3dinput init
	LPDIRECTINPUT8 pD3DInput = NULL;
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), 0x0800, IID_IDirectInput8, (LPVOID*)&pD3DInput, NULL)))
	{
		MessageBox(NULL, "D3DInput Init Failed!", "Alert", MB_OK);
		return E_FAIL;
	}
	if (FAILED(pD3DInput->CreateDevice(GUID_SysKeyboard, &g_pKeyBoardDevice, NULL)))
	{
		MessageBox(NULL, "D3DInputDevice Init Failed!", "Alert", MB_OK);
		return E_FAIL;
	}
	if (FAILED(g_pKeyBoardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		MessageBox(NULL, "SetCooperativeLevel Failed!", "Alert", MB_OK);
		return E_FAIL;
	}
	if (FAILED(g_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL, "SetDataFormat Failed!", "Alert", MB_OK);
		return E_FAIL;
	}
	g_pKeyBoardDevice->Acquire();
	SAFE_RELEASE(pD3DInput);
	
	//sprite init
	D3DXCreateSprite(g_pD3DD, &g_pD3DXSprite);

	//sound init
	DirectSoundCreate8(&DSDEVID_DefaultPlayback, &g_pDirectSound, NULL);
	g_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	return S_OK;
}

HRESULT Game_Init()
{
	g_pFPS = new FPSTools(g_pD3DD);
	g_pTimer = new Timer(60.0f);
	
	MenuItem* first = new MenuItem(g_pD3DD, "", "0");
	first->AddItem(new MenuItem(g_pD3DD, "开始游戏", "00"));
	MenuItem* second = new MenuItem(g_pD3DD, "帮    助", "01");
	second->AddItem(new MenuItem(g_pD3DD, "W 上 S 下 A 左 D 右\nJ 加速/确认\nK 跳跃/返回", "012"));
	first->AddItem(second);
	first->AddItem(new MenuItem(g_pD3DD, "退    出", "02"));
	
	g_Menu = new Menu(g_pD3DD, g_pDirectSound, first, "img/menu.png");
	return S_OK;
}

void Game_Render()
{
	if (FAILED(g_pKeyBoardDevice->GetDeviceState(sizeof(g_KeyBuffer), g_KeyBuffer)))
	{
		HRESULT hr = g_pKeyBoardDevice->Acquire();
		while (DIERR_INPUTLOST ==  hr)
		{
			g_pKeyBoardDevice->Acquire();
		}
		return ;
	}
	D3DCOLOR backColor = D3DCOLOR_XRGB(255, 255, 255);
	if (g_Map != NULL)
	{
		if (g_Map->GetMapStatus() == MAP_INIT)
			backColor = g_Map->GetBackColor();
		else
			backColor = g_Map->GetStory()->GetBackColor();
	}
	 
	g_pD3DD->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, backColor, 1.0f, 0);
	if (g_pD3DD->BeginScene())
	{
		if (g_Map != NULL)
		{
			g_Map->ImpactHandle(g_KeyBuffer);
			g_Map->Draw(g_pD3DXSprite);
			if (g_Map->GetMapStatus() == MAP_DESTORY)
			{
				SAFE_RELEASE(g_Map);
				g_Menu->Init();
				g_Menu->SetShow(true);
			}
		}
		if (g_Menu->GetShow())
		{
			g_Menu->Draw(g_pD3DXSprite);
			MenuItem* updateItem = g_Menu->GetItem()->Update(g_KeyBuffer);
			
			if (updateItem == NULL)
				PostQuitMessage(0);
			else
			{
				if (strcmp(updateItem->GetItemId(), "00") == 0)
				{
					g_Menu->SetShow(false);
					MapInit();
				}
				g_Menu->SetItem(updateItem);
				updateItem->Draw();
			}
		}

		g_pFPS->updateFPS();
		char fps[5];
		_itoa_s(g_pFPS->getFPS(), fps, 10);
		g_pFPS->getFont()->DrawTextA(NULL, fps, (int)strlen(fps), NULL, D3DFMT_UNKNOWN, g_pFPS->getColor());
		g_pD3DD->EndScene();
		g_pD3DD->Present(NULL, NULL, NULL, NULL);
	}
}

void MapInit()
{
	g_Map = new Map(g_pD3DD, g_pDirectSound, 5, 0.02f);
}

void Game_Release()
{
	SAFE_RELEASE(g_pD3DD);
	SAFE_RELEASE(g_pKeyBoardDevice);
	SAFE_RELEASE(g_Map);
	SAFE_RELEASE(g_Menu);
	SAFE_RELEASE(g_pD3DXSprite);
	SAFE_RELEASE(g_pDirectSound);
	SAFE_RELEASE(g_pFPS);
	SAFE_RELEASE(g_pKeyBoardDevice);
}