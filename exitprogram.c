#include "CZ.h"

_Analysis_noreturn_

DECLSPEC_NORETURN VOID WINAPI ExitProgram(_In_ DWORD dwExitCode, _In_opt_ HANDLE hData, _In_ HWND hWnd)
{
    CONST HANDLE hHeap = GetProcessHeap();
#ifdef MULTITHREADING_ENABLED
    EnterCriticalSection(&g_CS);
#endif
    if (g_wszzZapList != NULL)
    {
        HeapFree(hHeap, 0, g_wszzZapList);
        g_wszzZapList = NULL;
    }
#ifdef MULTITHREADING_ENABLED
    LeaveCriticalSection(&g_CS);
    DeleteCriticalSection(&g_CS);
#endif
    if (hData != NULL)
    {
        GlobalUnlock(hData);
    }
    RemoveClipboardFormatListener(hWnd);
    ExitProcess(dwExitCode);
}
