#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#define FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)

struct Vertex {
	float x, y, z;       // Position
	D3DXVECTOR3 normal;
	D3DCOLOR color;      // Vertex color
};

class CHGraphics
{
private:
	LPDIRECT3D9 pD3d;
	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer, floorBuffer;

public:
	CHGraphics() : pD3d(nullptr), pDevice(nullptr) {}
	~CHGraphics();

	int Init(HWND hwnd);
	void InitTriangle();
	void InitLighting();
	void InitFloor();
	void Render();
};