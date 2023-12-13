#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <sal.h>

#define EXTERN extern

EXTERN CONST WCHAR g_wszClassName[];
EXTERN WCHAR *g_wszzZapList;
#ifdef MULTITHREADING_ENABLED
EXTERN CRITICAL_SECTION g_CS;
#endif

typedef struct _ZAPLISTMETRICS
{
#ifdef _WIN64
    UINT64 uCchShortest; // Not presently used but still collected for potential future uses 
                         // One optimization I foresee is limiting iteration in the ZapClipboardText
                         // i.e. (for(i = 0; i < LENGTH - uCchShortest; i++)) 
    UINT64 uCchLongest;
#else
    UINT32 uCchShortest;
    UINT32 uCchLongest;
#endif
    UINT16 uListSize;
    WCHAR wcFirst;
    WCHAR wcSecond;
} ZAPLISTMETRICS, *PZAPLISTMETRICS;

_Analysis_noreturn_

DECLSPEC_NORETURN VOID WINAPI ExitProgram(_In_ DWORD dwExitCode, _In_opt_ HANDLE hData, _In_ HWND hWnd);
VOID WINAPI OnClipboardUpdate(_In_ HWND hWnd);

_Ret_range_(FALSE, TRUE)
_Success_(return)
_Check_return_

BOOL WINAPI RefreshConfiguration(_Out_ PZAPLISTMETRICS pZapListMetrics);
LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd);
VOID WINAPI ZapClipboardText(
    _In_
#ifdef _WIN64
    UINT64
#else
    UINT32
#endif
        uLen,
    _Inout_updates_(uLen) WCHAR *wszText
);
