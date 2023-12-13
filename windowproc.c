#include "CZ.h"

LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    switch (Msg)
    {
    case WM_CLIPBOARDUPDATE:
        OnClipboardUpdate(hWnd);
        break;
    default:
        return DefWindowProcW(hWnd, Msg, wParam, lParam);
    }
    return 0;
}
