#pragma once

#include <d3d11_1.h>
#include <wrl/client.h>
#include <thread>
#include "CWindows.h"

namespace p3d {

    template <typename T>
    using cComPtr = Microsoft::WRL::ComPtr<T>;

    void cSetupGraphics(const char* shaderRoot);
    void cCleanupGraphics();
    void cRecreateSwapChain();

    struct cGraphics
    {
        // D3D11 COM smart pointers 
        cComPtr<ID3D11Buffer>             tranformCBuf;
        cComPtr<ID3D11Texture2D>          frameBuffer;
        cComPtr<ID3D11RenderTargetView>   target;
        cComPtr<ID3D11DepthStencilView>   targetDepth;
        cComPtr<IDXGISwapChain>           swapChain;
        cComPtr<ID3D11DeviceContext>      imDeviceContext;  
        cComPtr<ID3D11Device>             device;

        // Meta
        const char*                       shaderRoot;
        std::thread::id                   threadID;
        
    };

}
