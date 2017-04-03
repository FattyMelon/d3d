// D3DStudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "d3dUtility.h"

extern IDirect3DDevice9* Device;
extern float Width;
extern float Height;

#ifdef LIGHTING

IDirect3DVertexBuffer9 *Pyramid = 0;	//vertex buffer to store
D3DXMATRIX WorldMatrix;

struct Vertex
{
	Vertex() {}

	Vertex(float x, float y, float z, float nx, float ny, float nz)
	{
		_x = x;  _y = y;	_z = z;
		_nx = nx; _ny = ny; _nz = nz;
	}
	float  _x, _y, _z;
	float _nx, _ny, _nz;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

bool Setup()
{
	//
	// Turn on lighting.
	//
	Device->SetRenderState(D3DRS_LIGHTING, true);

	//
	// Create the vertex buffer for the pyramid.
	//

	Device->CreateVertexBuffer(
		12 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&Pyramid,
		0);

	//
	// Fill the vertex buffer with pyramid data.
	//

	Vertex* v;
	Pyramid->Lock(0, 0, (void**)&v, 0);

	// front face
	v[0] = Vertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
	v[1] = Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
	v[2] = Vertex(1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);

	// left face
	v[3] = Vertex(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
	v[4] = Vertex(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
	v[5] = Vertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

	// right face
	v[6] = Vertex(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
	v[7] = Vertex(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
	v[8] = Vertex(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

	// back face
	v[9] = Vertex(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	v[10] = Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
	v[11] = Vertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);

	Pyramid->Unlock();

	//
	// Create and set the material.
	//

	D3DMATERIAL9 mtrl;
	mtrl.Ambient = d3d::WHITE;
	mtrl.Diffuse = d3d::WHITE;
	mtrl.Specular = d3d::WHITE;
	mtrl.Emissive = d3d::BLACK;
	mtrl.Power = 5.0f;

	Device->SetMaterial(&mtrl);

	//
	// Setup a directional light.
	//

	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = d3d::WHITE;
	dir.Specular = d3d::WHITE * 0.3f;
	dir.Ambient = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	//
	// Set and Enable the light.
	//

	Device->SetLight(0, &dir);
	Device->LightEnable(0, true);

	//
	// Turn on specular lighting and instruct Direct3D
	// to renormalize normals.
	//

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Position and aim the camera.
	//

	D3DXVECTOR3 pos(0.0f, 1.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;

}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Pyramid);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		// 
		// Update the scene: Rotate the pyramid.
		//

		D3DXMATRIX yRot;

		static float y = 0.0f;

		D3DXMatrixRotationY(&yRot, y);
		y += timeDelta;

		if (y >= 6.28f)
			y = 0.0f;

		Device->SetTransform(D3DTS_WORLD, &yRot);

		//
		// Draw the scene:
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, Pyramid, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}


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

#endif