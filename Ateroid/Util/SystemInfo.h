#pragma once

namespace ASTEROID_NAMESPACE
{
    class SystemInfo
    {
    public:
        ASTEROID_NO_DEFAULT_CTOR(SystemInfo)
        ASTEROID_NON_COPYABLE(SystemInfo)

        static bool Initialize();

        static uint32_t ProcessorsCount() { return _SystemInfo.dwNumberOfProcessors; }

    private:
        static void LogInfo();

    private:
        static SYSTEM_INFO _SystemInfo;
        static ULONGLONG _TotalMemoryKilobytes;
    };
}