// D3DStudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "d3dUtility.h"

IDirect3DDevice9* Device = 0;
float Width = 640;
float Height = 480;

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
D3DXMATRIX WorldMatrix;

struct ColorVertex
{
	ColorVertex() {}
	ColorVertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x; _y = y; _z = z; _color = c;
	}

	float _x, _y, _z;
	D3DCOLOR _color;
	static CONST DWORD FVF;
};

const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;


bool Setup()
{
	Device->CreateVertexBuffer(
		3 * sizeof(ColorVertex),
		D3DUSAGE_WRITEONLY,
		ColorVertex::FVF,
		D3DPOOL_MANAGED,
		&Triangle,
		0);

	ColorVertex* vertices;
	Triangle->Lock(0, 0, (void **)&vertices, 0);

	vertices[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255,   0,   0));
	vertices[1] = ColorVertex( 0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(  0, 255,   0));
	vertices[2] = ColorVertex( 1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(  0,   0, 255));
	
	Triangle->Unlock();

	//set the projection matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, 
		(float)Width / (float) Height,
		1.0f,
		100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//set wireframe mode render state.
	Device->SetRenderState(D3DRS_LIGHTING, false);
	Device->SetRenderState(D3DRS_ZENABLE, true);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		//D3DXMATRIX Ry, Rz;
		//D3DXMatrixRotationY(&Ry, 3.14f / 4.0f);

		//static float z = 0.0f;
		//D3DXMatrixRotationZ(&Rz, z);
		//z += timeDelta;

		//if (z >= 6.28f)
		//	z = 0.0f;

		//D3DXMATRIX p = Rz * Ry;
		//Device->SetTransform(D3DTS_WORLD, &p);

		//draw the Scene
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, Triangle, 0, sizeof(ColorVertex));
		Device->SetFVF(ColorVertex::FVF);

		D3DXMatrixTranslation(&WorldMatrix, 0.f, 0.0f, 0.0f);
		Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

		Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		// Draw one triangle.
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		D3DXMatrixTranslation(&WorldMatrix, 1.0f, 0.0f, 0.0f);
		Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

		Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


		Device->EndScene();
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
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
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
