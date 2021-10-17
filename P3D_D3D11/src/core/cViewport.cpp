#include "cViewport.h"
#include "p3d.h"
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace p3d {

    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void cInitializeViewport(int width, int height)
    {
        GContext->viewport.width = width;
        GContext->viewport.height = height;

        WNDCLASSEX wc = { 0 };
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = "VGS Window";
        wc.hIconSm = nullptr;
        RegisterClassEx(&wc);

        // calculate window size based on desired client region size
        RECT wr;
        wr.left = 100;
        wr.right = width + wr.left;
        wr.top = 100;
        wr.bottom = height + wr.top;
        AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPED | WS_THICKFRAME, FALSE);

        // create window & get hWnd
        GContext->viewport.hWnd = CreateWindow(
            "VGS Window", "Virtual Gauge System",
            WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_OVERLAPPED | WS_THICKFRAME,
            0, 0, wr.right - wr.left, wr.bottom - wr.top,
            nullptr, nullptr, GetModuleHandle(nullptr), nullptr);


        ShowWindow(GContext->viewport.hWnd, SW_SHOWDEFAULT);
    }
    
    std::optional<int> cProcessViewportEvents()
    {
        MSG msg;
        
        // while queue has messages, remove and dispatch them (but do not block on empty queue)
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // check for quit because peekmessage does not signal this via return val
            if (msg.message == WM_QUIT)
                return (int)msg.wParam;
        }

        // return empty optional when not quitting app
        return {};
    }

    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {

        case WM_SIZE:
            if (wParam != SIZE_MINIMIZED)
            {
                RECT rect;
                RECT crect;
                int awidth = 0;
                int aheight = 0;
                int cwidth = 0;
                int cheight = 0;
                if (GetWindowRect(hWnd, &rect))
                {
                    awidth = rect.right - rect.left;
                    aheight = rect.bottom - rect.top;
                }

                if (GetClientRect(hWnd, &crect))
                {
                    cwidth = crect.right - crect.left;
                    cheight = crect.bottom - crect.top;
                }

                // I believe this are only used for the error logger
                //m_width = (UINT)LOWORD(lParam);
                //m_height = (UINT)HIWORD(lParam);
                GContext->viewport.width = cwidth;
                GContext->viewport.height = cheight;
                GContext->viewport.resized = true;

            }
            return 0;

        case WM_DESTROY:

            PostQuitMessage(0);
            break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    
}
