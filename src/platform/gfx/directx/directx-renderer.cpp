#include "include/ctetdx.h"
#include <DirectXMath.h>

struct Vertex {
    DirectX::XMFLOAT3 position;
};

void dxRenderer_init(DxRenderer *dxR, HWND window, int width, int height) {
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    
    
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.SampleDesc.Count = 4;
    swapChainDesc.Windowed = TRUE;
    
    D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &dxR->swapChain,
            &dxR->device,
            nullptr,
            &dxR->deviceCon
    );
    
    // create a render target view
    ID3D11Texture2D *backBuffer = nullptr;
    dxR->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &backBuffer);
    dxR->device->CreateRenderTargetView(backBuffer, nullptr, &dxR->renderTarget);
    backBuffer->Release();

    // set the render target
    dxR->deviceCon->OMSetRenderTargets(1, &dxR->renderTarget, nullptr);

    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float) width;
    viewport.Height = (float) height;

    dxR->deviceCon->RSSetViewports(1, &viewport);
}

void dxRenderer_cleanup(DxRenderer *dxR) {
    dxR->swapChain->Release();
    dxR->renderTarget->Release();
    dxR->device->Release();
    dxR->deviceCon->Release();
}

void dxRenderer_renderFrame(DxRenderer *dxR) {
    float clearColor[] = {1.0f, 0.0f, 1.0f, 1.0f};
    dxR->deviceCon->ClearRenderTargetView(dxR->renderTarget, clearColor);
    dxR->swapChain->Present(0, 0);
}