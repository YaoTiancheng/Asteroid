#pragma once

namespace ASTEROID_NAMESPACE
{
    /**
     *  Log types for Debug::LogXXX functions.
     */
    enum class ELogType
    {
        eInfo, eWarning, eError, eAssert
    };


    /**
     *  Utility functions for debugging.\n
     *  Debug provides several functions for logging. Those functions outputs a message string to the debugger
     *  and stderr, which is redirected to a local log file.
     */
    class Debug
    {
    public:
        ASTEROID_NO_DEFAULT_CTOR(Debug)
        ASTEROID_NON_COPYABLE(Debug)

        /**
         *  Initialize debug context. \n
         *  Basiclly it load symbol files of the current process which is used for stack tracing.
         *  And it redirect the stderr stream to the LastRun.log log file.\n
         *  This function should be called at the engine start as early as possible to provide the logging
         *  and stack tracing feature that other component of the engine might rely on.
         */
        static void Initialize();

        /**
         *  Finalize debug context.\n
         *  Flush the stderr stream and clear the loaded symbol file.
         */
        static void Finalize();

        /**
         *  Output a message string to log.
         *  @param type
         *      The type of this message.
         *  @param str
         *      The message string output to log.
         *  @param stackTrace
         *      Output a stack trace or not.
         */
        static void Log(ELogType type, const char* str, bool stackTrace = false);

        /**
         *  Output a formatted message string to log.
         *  @param type
         *      The type of this message.
         *  @param str
         *      The message string output to log.
         *  @param stackTrace
         *      Output a stack trace or not.
         *  @remark
         *      The maximum message string length is indicated by @see kMaxLogStringByteLength
         */
        static void LogFormat(ELogType type, const char* format, bool stackTrace, ...);

        /**
         *  Cause a line break in the attached debugger.
         */
        static void Break();

        /**
         *  Raise an assertion failure message.
         */
        static void Assert(const wchar_t* expression, const wchar_t* file, int line);

    public:
        /** Maximum message string length for formatted log output each time. */
        static const uint32_t kMaxLogStringByteLength = 1024;
    };


    /**
     *  Create a instance of this class to capture the current stack frames.
     *  And it provides function to print the stack frames captured to an output stream.
     */
    class StackTrace
    {
        friend std::ostream& operator<<(std::ostream& o, const StackTrace& st);

    public:
        /**
         *  Construct a stack trace object and capture the current stack frames.
         */
        StackTrace();

    private:
        static const int kMaxFrameCount = 62;
        int m_FrameCount;
        void* m_Frames[kMaxFrameCount];
    };
}


#ifdef _DEBUG
    /** Assert and output a log if failed */
    #define ASTEROID_ASSERT(c, m)                                   \
        do {                                                        \
            if (!(c)) {                                             \
                Debug::Log(ELogType::eAssert, m, true);             \
                Debug::Assert(_CRT_WIDE_(#c), __FILEW__, __LINE__); \
            }                                                       \
        } while(false)

    /** Assert and output a formatted log if failed */
    #define ASTEROID_ASSERT_F(c, format, ...)                                   \
        do {                                                                    \
            if (!(c)) {                                                         \
                Debug::LogFormat(ELogType::eAssert, format, true, __VA_ARGS__); \
                Debug::Assert(_CRT_WIDE_(#c), __FILEW__, __LINE__);             \
            }                                                                   \
        } while(false)
#else
    #define ASTEROID_ASSERT(c, m) ((void)0)
    #define ASTEROID_ASSERT_F(c, m) ((void)0)
#endif

#ifndef ASTEROID_NO_LOG_INFO
    /** Output an info log */
    #define ASTEROID_LOG_INFO(STR) do { Debug::Log(ELogType::eInfo, STR, false); } while (false)
    /** Output a formatted info log */
    #define ASTEROID_LOG_INFO_F(STR, ...) do { Debug::LogFormat(ELogType::eInfo, STR, false, __VA_ARGS__); } while (false)
    /** Output an info log with stack trace */
    #define ASTEROID_LOG_INFO_TRACE(STR) do { Debug::Log(ELogType::eInfo, STR, true); } while (false)
    /** Output a formatted info log with stack trace*/
    #define ASTEROID_LOG_INFO_TRACE_F(STR, ...) do { Debug::LogFormat(ELogType::eInfo, STR, true, __VA_ARGS__); } while (false)
#else 
    #define ASTEROID_LOG_INFO(STR) ((void)0)
    #define ASTEROID_LOG_INFO_F(STR, ...) ((void)0)
    #define ASTEROID_LOG_INFO_TRACE(STR) ((void)0)
    #define ASTEROID_LOG_INFO_TRACE_F(STR, ...) ((void)0)
#endif

#ifndef ASTEROID_NO_LOG_WARNING
    /** Output an warning log */
    #define ASTEROID_LOG_WARNING(STR) do { Debug::Log(ELogType::eWarning, STR, false); } while (false)
    /** Output a formatted warning log */
    #define ASTEROID_LOG_WARNING_F(STR, ...) do { Debug::LogFormat(ELogType::eWarning, STR, false, __VA_ARGS__); } while (false)
    /** Output an warning log with stack trace */
    #define ASTEROID_LOG_WARNING_TRACE(STR) do { Debug::Log(ELogType::eWarning, STR, true); } while (false)
    /** Output a formatted warning log with stack trace*/
    #define ASTEROID_LOG_warning_TRACE_F(STR, ...) do { Debug::LogFormat(ELogType::eWarning, STR, true, __VA_ARGS__); } while (false)
#else 
    #define ASTEROID_LOG_WARNING(STR) ((void)0)
    #define ASTEROID_LOG_WARNING_F(STR, ...) ((void)0)
    #define ASTEROID_LOG_WARNING_TRACE(STR) ((void)0)
    #define ASTEROID_LOG_warning_TRACE_F(STR, ...) ((void)0)
#endif

#ifndef ASTEROID_NO_LOG_ERROR
    /** Output an error log */
    #define ASTEROID_LOG_ERROR(STR) do { Debug::Log(ELogType::eError, STR, false); } while (false)
    /** Output a formatted error log */
    #define ASTEROID_LOG_ERROR_F(STR, ...) do { Debug::LogFormat(ELogType::eError, STR, false, __VA_ARGS__); } while (false)
    /** Output an error log with stack trace */
    #define ASTEROID_LOG_ERROR_TRACE(STR) do { Debug::Log(ELogType::eError, STR, true); } while (false)
    /** Output a formatted error log with stack trace */
    #define ASTEROID_LOG_ERROR_TRACE_F(STR, ...) do { Debug::LogFormat(ELogType::eError, STR, true, __VA_ARGS__); } while (false)
#else 
    #define ASTEROID_LOG_ERROR(STR) ((void)0)
    #define ASTEROID_LOG_ERROR_F(STR, ...) ((void)0)
    #define ASTEROID_LOG_ERROR_TRACE(STR) ((void)0)
    #define ASTEROID_LOG_ERROR_TRACE_F(STR, ...) ((void)0)
#endif

