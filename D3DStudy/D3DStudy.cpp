// D3DStudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "d3dUtility.h"

IDirect3DDevice9* Device = 0;

#ifdef DEFAUT
bool Setup()
{
	return true;
}

void Cleanup()
{

}

bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		Device->Present(0, 0, 0, 0);
	}
	return true;
}

#endif

#ifdef TRIANGLE

IDirect3DVertexBuffer9 *Triangle = 0;	//vertex buffer to store

bool Setup()
{
	return true;
}

void Cleanup()
{

}

bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		Device->Present(0, 0, 0, 0);
	}
	return true;
}

#endif

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow) 
{
	if (!d3d::InitD3D(hInstance,
		640, 480, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);
	
	Cleanup();

	Device->Release();

	return 0;
}
