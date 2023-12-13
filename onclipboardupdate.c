#include "CZ.h"

VOID WINAPI OnClipboardUpdate(_In_ HWND hWnd)
{
    CONST HANDLE hHeap = GetProcessHeap();
    HANDLE hData = NULL;
    WCHAR *wszText = NULL;
    WCHAR *wszNewText = NULL;
    HRESULT hr = STRSAFE_E_INVALID_PARAMETER;
#ifdef _WIN64
    UINT64
#else
    UINT32
#endif
        uLen;

    if (hWnd == GetClipboardOwner())
    {
        return;
    }

    if (!OpenClipboard(NULL))
    {
        return;
    }
    hData = GetClipboardData(CF_UNICODETEXT);
    if (NULL == hData)
    {
        CloseClipboard();
        return;
    }
    wszText = (WCHAR *) GlobalLock(hData);
    if (wszText != NULL)
    {
        GlobalUnlock(hData);
        hr = StringCbLengthW(wszText, STRSAFE_MAX_CCH * sizeof(WCHAR), &uLen);
    }

    if (SUCCEEDED(hr))
    {
        HGLOBAL hCB = NULL;
        uLen += sizeof(WCHAR);
        hCB = GlobalAlloc(GMEM_MOVEABLE, uLen);
        if (hCB != NULL)
        {
            wszNewText = (WCHAR *) GlobalLock(hCB);
            if (wszNewText != NULL)
            {
                CopyMemory(wszNewText, wszText, uLen);
                ZapClipboardText(uLen / sizeof(WCHAR), wszNewText);
                GlobalUnlock(hCB);
            }
            EmptyClipboard();
            SetClipboardData(CF_UNICODETEXT, hCB);
        }
    }

    CloseClipboard();
}
