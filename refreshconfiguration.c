#include "CZ.h"

_Ret_range_(FALSE, TRUE)
_Success_(return)
_Check_return_

BOOL WINAPI RefreshConfiguration(_Out_ PZAPLISTMETRICS pZapListMetrics)
{
    CONST HANDLE hHeap = GetProcessHeap();
    LSTATUS ls;
    HKEY hKey = NULL;
#ifdef _WIN64
    UINT64
#else
    UINT32
#endif
        uLen;
    HRESULT hr;
    DWORD dwZapChar, dwCB;
    ZAPLISTMETRICS zlm;
    WCHAR *pStart = NULL;
#ifdef MULTITHREADING_ENABLED
    BOOL fCS = FALSE;
#endif
    ZeroMemory(&zlm, sizeof(ZAPLISTMETRICS));
    zlm.uCchShortest = UINT_MAX;

    ls = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\GovindParmar\\ClipboardZapper",
        0U, NULL, 0, KEY_QUERY_VALUE, NULL, &hKey, NULL);
    if (ls != ERROR_SUCCESS)
    {
        goto cleanup;
    }

    dwCB = sizeof(DWORD);
    ls = RegQueryValueExW(hKey, L"ZapChar", NULL, NULL, (LPBYTE) &dwZapChar, &dwCB);
    if (ls != ERROR_SUCCESS)
    {
        goto cleanup;
    }

    zlm.wcFirst = (WCHAR) (DWORD) LOWORD(dwZapChar);
    zlm.wcSecond = (WCHAR) (DWORD) HIWORD(dwZapChar);

#ifdef MULTITHREADING_ENABLED
    EnterCriticalSection(&g_CS);
    fCS = TRUE;
#endif

    if (g_wszzZapList != NULL)
    {
        HeapFree(hHeap, 0, g_wszzZapList);
        g_wszzZapList = NULL;
    }

    ls = RegQueryValueExW(hKey, L"ZapList", NULL, NULL, NULL, &dwCB);
    if (ls != ERROR_SUCCESS)
    {
        goto cleanup;
    }

    g_wszzZapList = (WCHAR *) HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwCB + (sizeof(WCHAR) * 10));
    if (NULL == g_wszzZapList)
    {
        ls = (LSTATUS) ERROR_OUTOFMEMORY;
        goto cleanup;
    }

#ifdef MULTITHREADING_ENABLED
    LeaveCriticalSection(&g_CS);
    fCS = FALSE;
#endif

    pStart = g_wszzZapList;
    ls = RegQueryValueExW(hKey, L"ZapList", NULL, NULL, (LPBYTE) g_wszzZapList, &dwCB);
    if (ls != ERROR_SUCCESS)
    {
        goto cleanup;
    }

    while (*g_wszzZapList)
    {
        zlm.uListSize++;
        hr = StringCchLengthW(g_wszzZapList, dwCB / sizeof(WCHAR), &uLen);
        if (FAILED(hr))
        {
            ls = (DWORD) ERROR_INVALID_DATA;
            goto cleanup;
        }

        zlm.uCchLongest = max(uLen, zlm.uCchLongest);
        zlm.uCchShortest = min(uLen, zlm.uCchShortest);
        g_wszzZapList = g_wszzZapList + uLen + 1;
    }

    g_wszzZapList = pStart;
    CopyMemory(pZapListMetrics, &zlm, sizeof(ZAPLISTMETRICS));
    ls = ERROR_SUCCESS;
cleanup:
#ifdef MULTITHREADING_ENABLED
    if (fCS)
    {
        LeaveCriticalSection(&g_CS);
        fCS = FALSE;
    }
#endif
    if (hKey != NULL)
    {
        RegCloseKey(hKey);
        hKey = NULL;
    }

    SetLastError((DWORD) ls);
    return ERROR_SUCCESS == ls;
}