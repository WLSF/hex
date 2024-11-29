#include "CHGraphics.h"

CHGraphics::~CHGraphics() {
    if (vertexBuffer) vertexBuffer->Release();
    if (floorBuffer) floorBuffer->Release();
    if (pDevice) pDevice->Release();
    if (pD3d) pD3d->Release();
}

int CHGraphics::Init(HWND hWnd) {
    pD3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3d)
        return 1;

    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = 800;
    d3dpp.BackBufferHeight = 600;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
    {
        return 2;
    }

    return 0;
}

void CHGraphics::InitTriangle() {
    pDevice->CreateVertexBuffer(3 * sizeof(Vertex), 0, FVF,
        D3DPOOL_MANAGED, &vertexBuffer, NULL);

    Vertex* vertices;
    vertexBuffer->Lock(0, 0, (void**)&vertices, 0);
    vertices[0] = { 0.0f,  1.0f,  0.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(255, 0, 0) }; // Top
    vertices[1] = { 1.0f, -1.0f,  0.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(0, 255, 0) }; // Bottom-right
    vertices[2] = { -1.0f, -1.0f, 0.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(0, 0, 255) }; // Bottom-left
    vertexBuffer->Unlock();
}

void CHGraphics::InitLighting() {
    D3DLIGHT9 light = {};
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // White diffuse light
    light.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); // Specular highlights
    light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);  // Ambient light
    light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.5f);   // Light direction

    pDevice->SetLight(0, &light);
    pDevice->LightEnable(0, TRUE);  // Enable light 0
    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE); // Enable specular highlights
}

void CHGraphics::InitFloor() {
    pDevice->CreateVertexBuffer(6 * sizeof(Vertex), 0, FVF,
        D3DPOOL_MANAGED, &floorBuffer, NULL);

    Vertex* vertices;
    floorBuffer->Lock(0, 0, (void**)&vertices, 0);
    vertices[0] = { -5.0f, -1.0f,  5.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(100, 100, 100) };
    vertices[1] = { 5.0f, -1.0f,  5.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(100, 100, 100) };
    vertices[2] = { -5.0f, -1.0f, -5.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(100, 100, 100) };
    vertices[3] = { 5.0f, -1.0f,  5.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(100, 100, 100) };
    vertices[4] = { 5.0f, -1.0f, -5.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(100, 100, 100) };
    vertices[5] = { -5.0f, -1.0f, -5.0f, { 0.0f, 1.0f, 0.0f }, D3DCOLOR_XRGB(100, 100, 100) };
    floorBuffer->Unlock();
}

void CHGraphics::Render() {
    if (!pDevice) return;

    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);

    if (SUCCEEDED(pDevice->BeginScene())) {
        // Set transforms
        D3DXVECTOR3 eye(0.0f, 3.0f, -6.0f);
        D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
        D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
        D3DXMATRIX viewMatrix, projMatrix, worldMatrix;
        D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
        D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI / 4, 800.0f / 600.0f, 1.0f, 100.0f);
        D3DXMatrixIdentity(&worldMatrix);

        pDevice->SetTransform(D3DTS_WORLD, &worldMatrix);
        pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
        pDevice->SetTransform(D3DTS_PROJECTION, &projMatrix);

        // Draw floor
        pDevice->SetStreamSource(0, floorBuffer, 0, sizeof(Vertex));
        pDevice->SetFVF(FVF);
        pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

        // Draw triangle
        pDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
        pDevice->SetFVF(FVF);
        pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

        //pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        pDevice->EndScene();
    }

    pDevice->Present(NULL, NULL, NULL, NULL);
}