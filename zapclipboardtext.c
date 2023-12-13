#include "CZ.h"

VOID WINAPI ZapClipboardText(
    _In_
#ifdef _WIN64
    UINT64
#else
    UINT32
#endif
    uLen,
    _Inout_updates_(uLen) WCHAR *wszText
)
{
    WCHAR *pStart;
    ZAPLISTMETRICS zlm;
#ifdef _WIN64
    UINT64
#else
    UINT32
#endif
        i = 0, i2 = 0;

    (VOID) RefreshConfiguration(&zlm);
    pStart = g_wszzZapList;
    for (; i < uLen; i++)
    {
        HRESULT hr;
        UINT uZapLen;

        g_wszzZapList = pStart;
        while (*g_wszzZapList)
        {
            hr = StringCchLengthW(g_wszzZapList, zlm.uCchLongest + 1, &uZapLen);
            if (SUCCEEDED(hr)) // If sane code, this is always true
            {
                if (i + uZapLen < uLen && wcsncmp(g_wszzZapList, wszText + i, uZapLen) == 0)
                {
                    for (i2 = 0; i2 < uZapLen; i2++)
                    {
                        if (zlm.wcSecond)
                        {
                            wszText[i + i2] = (i2 & 1) ? zlm.wcFirst : zlm.wcSecond;
                        }
                        else
                        {
                            wszText[i + i2] = zlm.wcFirst;
                        }
                    }
                }
            }
            g_wszzZapList = g_wszzZapList + uZapLen + 1;
        }
    }

    g_wszzZapList = pStart;
}
