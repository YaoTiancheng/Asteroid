#include "Precompile.h"
#include "Asteroid.h"
#include "SystemInfo.h"
#include "Debug.h"
#include "WindowsUtil.h"

namespace ASTEROID_NAMESPACE
{
    SYSTEM_INFO SystemInfo::_SystemInfo;
    ULONGLONG   SystemInfo::_TotalMemoryKilobytes;

    bool SystemInfo::Initialize()
    {
        LogInfo();

        return true;
    }

    void SystemInfo::LogInfo()
    {
    }
}
