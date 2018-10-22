#include "Precompile.h"
#include "Asteroid.h"
#include "Debug.h"
#include "String.h"


namespace ASTEROID_NAMESPACE
{
    static const char kLogFilepath[] = ".\\LastRun.log";
    // A temopary string buffer for string formatting.
    static char gLogStringFormatBuffer[Debug::kMaxLogStringByteLength] = { 0 };

    static const char* kLogTypeNames[] =
    {
        "[Info]", "[Warning]", "[Error]", "[Assert]"
    };

    template<class _Elem, class _Traits> 
    std::basic_ostream<_Elem, _Traits>& __CLRCALL_OR_CDECL LogTimeStamp(std::basic_ostream<_Elem, _Traits>& _Ostr)
    {	
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        _Ostr << std::setfill('0') 
            << std::setw(2) << now->tm_hour
            << ':' << std::setw(2) << now->tm_min 
            << ':' << std::setw(2) << now->tm_sec << ' ';
        return _Ostr;
    }

    static void InternalLogFormat(ELogType type, const char* format, bool forceStackTrace, va_list args)
    {
        std::ostringstream ss;
        ss << kLogTypeNames[(int)type] << ' ';
        vsnprintf(gLogStringFormatBuffer, Debug::kMaxLogStringByteLength, format, args);
        ss << gLogStringFormatBuffer << std::endl;

        // Print stack trace if stack trace is forced or log type is an error or an assertion.
        if (forceStackTrace)
        {
            StackTrace st;
            ss << st;
        }

        const std::string& str = ss.str();
        OutputDebugStringA(str.c_str());

        std::cerr << LogTimeStamp << str;
    }

    static void InternalLog(ELogType type, const char* buffer, bool forceStackTrace)
    {
        std::ostringstream ss;
        ss << kLogTypeNames[(int)type] << ' ' << buffer << std::endl;

        // Print stack trace if stack trace is forced or log type is an error or an assertion.
        if (forceStackTrace)
        {
            StackTrace st;
            ss << st;
        }

        const std::string& str = ss.str();
        OutputDebugStringA(str.c_str());

        std::cerr << LogTimeStamp << str;
    }

    void Debug::Initialize()
    {
        HANDLE process = GetCurrentProcess();
        SymSetOptions(SYMOPT_LOAD_LINES); 
        SymInitialize(process, NULL, TRUE);  

        // Redirect std::cerr to log file
        freopen(kLogFilepath, "w", stderr);
    }

    void Debug::Finalize()
    {
        fflush(stderr);

        HANDLE process = GetCurrentProcess();
        SymCleanup(process);
    }

    void Debug::Log(ELogType type, const char* str, bool stackTrace)
    {
        InternalLog(type, str, stackTrace);
    }

    void Debug::LogFormat(ELogType type, const char* format, bool stackTrace, ...)
    {
        va_list args;
        va_start(args, stackTrace);
        InternalLogFormat(type, format, stackTrace, args);
        va_end(args);
    }

    void Debug::Break()
    {
        DebugBreak();
    }

    void Debug::Assert(const wchar_t* expression, const wchar_t* file, int line)
    {
        _wassert(expression, file, line); 
    }

    StackTrace::StackTrace()
    {
        m_FrameCount = CaptureStackBackTrace(1, kMaxFrameCount, m_Frames, NULL);
    }

    std::ostream& operator<<(std::ostream& o, const StackTrace& st)
    {
        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
        PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbolBuffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        IMAGEHLP_LINE64 line; // define the IMAGEHLP_LINE64
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        HANDLE process = GetCurrentProcess();
        for (int i = 0; i < st.m_FrameCount; i++)
        {
            DWORD64 dw64Displacement;
            SymFromAddr(process, (DWORD64)(st.m_Frames[i]), &dw64Displacement, symbol); // get symbol info
            o << symbol->Name;
            DWORD dwDisplacement;
            if (SymGetLineFromAddr64(process, (DWORD64)(st.m_Frames[i]), &dwDisplacement, &line)) // get line info
                o << ' ' << line.FileName << ", line " << line.LineNumber;
            o << std::endl;
        }
        return o;
    }
}
