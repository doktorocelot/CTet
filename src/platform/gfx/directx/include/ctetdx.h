#pragma once
#include <Windows.h>
#include <d3d11.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    IDXGISwapChain *swapChain;
    ID3D11Device *device;
    ID3D11DeviceContext *deviceCon;
    ID3D11RenderTargetView *renderTarget;
} DxRenderer;


void dxRenderer_init(DxRenderer *dxR, HWND window, int width, int height);
void dxRenderer_cleanup(DxRenderer *dxR);

void dxRenderer_renderFrame(DxRenderer *dxR);

#ifdef __cplusplus
}
#endif