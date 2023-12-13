#include "CZ.h"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
    HWND hWnd;
    MSG Msg;
    WNDCLASSEXW wcex;

    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.hInstance = hInstance;
    wcex.lpszClassName = g_wszClassName;
    wcex.lpfnWndProc = WindowProc;

    if (!RegisterClassExW(&wcex))
    {
        MessageBoxW(NULL, L"Could not register message-only window class", L"Clipboard Zapper", MB_OK | MB_ICONSTOP);
        return -1;
    }

    hWnd = CreateWindowW(g_wszClassName, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);
    if (NULL == hWnd)
    {
        MessageBoxW(NULL, L"Could not create message-only window class", L"Clipboard Zapper", MB_OK | MB_ICONSTOP);
        return -1;
    }

    if (!AddClipboardFormatListener(hWnd))
    {
        MessageBoxW(NULL, L"Could not register clipboard listener", L"Clipboard Zapper", MB_OK | MB_ICONSTOP);
        return -1;
    }

#ifdef MULTITHREADING_ENABLED
    if (!InitializeCriticalSectionAndSpinCount(&g_CS, 0x000001F4U))
    {
        MessageBoxW(NULL, L"Could not initialize critical section for synchronizing refresh operations", L"Clipboard Zapper", MB_OK | MB_ICONSTOP);
        return -1;
    }
#endif
    while (GetMessageW(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }
#ifdef MULTITHREADING_ENABLED
    DeleteCriticalSection(&g_CS);
#endif
    RemoveClipboardFormatListener(hWnd);
    return Msg.wParam;
}
