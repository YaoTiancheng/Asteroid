#include "Precompile.h"
#include "Asteroid.h"
#include "WindowsUtil.h"

namespace ASTEROID_NAMESPACE
{
    String WindowsUtil::GetLastErrorString()
    {
        String result;

        DWORD dw = GetLastError();

        LPVOID lpMsgBuf = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (char*)&lpMsgBuf,
            0, NULL);

        result.assign((const char*)lpMsgBuf);
        LocalFree(lpMsgBuf);

        return result;
    }
}
