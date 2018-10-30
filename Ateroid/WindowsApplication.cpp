#include "Precompile.h"
#include "Asteroid.h"
#include "WindowsApplication.h"
#include "Util/STLAllocator.h"
#include "Util/ConsoleVariable.h"
#include "Util/Debug.h"
#include "Util/PlayerPrefs.h"
#include "Util/SystemInfo.h"

namespace ASTEROID_NAMESPACE
{
    static const TCHAR kMainWindowClassName[] = L"MainWindow";

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static ATOM RegisterMainWindowClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ATEROID));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = kMainWindowClassName;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
        return RegisterClassExW(&wcex);
    }

    static HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
    {
        ASTEROID_LOG_INFO_F("Create main window with nWidth:%d, nHeight:%d", CW_USEDEFAULT, CW_USEDEFAULT);
        HWND hWnd = CreateWindowW(kMainWindowClassName, L"", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

        if (!hWnd)
        {
            ASTEROID_LOG_ERROR("Create main window failed.");
            return NULL;
        }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return hWnd;
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        return WindowsApplication::Singleton()->WndProc(hWnd, message, wParam, lParam);
    }

    WindowsApplication* WindowsApplication::_Singleton = nullptr;

    WindowsApplication::WindowsApplication(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int cmdShow)
        : m_hInstance(hInstance), m_CmdShow(cmdShow)
    {
#ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

        ASTEROID_ASSERT(_Singleton == nullptr, "There is already a singleton created.");
        _Singleton = this;
    }

    WindowsApplication::~WindowsApplication()
    {
        Finalize();
        _Singleton = nullptr;
    }

    int WindowsApplication::Run()
    {
        if (!Initialize())
            return 1;

        int retCode = MainMessageLoop();
        ASTEROID_LOG_INFO_F("Exit with return code %d.", retCode);
        return retCode;
    }

    LRESULT WindowsApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    bool WindowsApplication::Initialize()
    {
        // Initialize the debug system, load symbol files, redirect stderr, etc.
        Debug::Initialize();

        if (!SystemInfo::Initialize())
        {
            ASTEROID_LOG_ERROR("Application init failed: SystemInfo init failed.");
            return false;
        }

        PlayerPrefs::Create();
        if (!PlayerPrefs::Singleton()->Load())
        {
            // PlayerPrefs load failed for some reason. Just print a log and continue.
            ASTEROID_LOG_INFO("PlayerPrefs::Load failed.");
        }

        ConsoleVariableManager::Create(PlayerPrefs::Singleton());

        RegisterMainWindowClass(m_hInstance);
        m_hWnd = CreateMainWindow(m_hInstance, m_CmdShow);
        if (m_hWnd == NULL)
            return false;

        ConsoleVariable<std::string>::SharedPtrType var = ConsoleVariable<std::string>::Create("Hello", true, "Fuck you");
        ConsoleVariable<std::string>::SharedPtrType var1 = ConsoleVariable<std::string>::Create("Hello1", false, "Fuck you too");
        ConsoleVariable<int32_t>::SharedPtrType var2 = ConsoleVariable<int32_t>::Create("Hello2");
        ConsoleVariable<float>::SharedPtrType var3 = ConsoleVariable<float>::Create("Hello3");
        ConsoleVariable<float>::SharedPtrType var4 = ConsoleVariable<float>::Create("Hello4");
        int32_t a = (*var2) + 6;
        *var2 = a;
        *var3 = *var2 + *var3;

        VectorA<int, 16> haha;
        haha.reserve(64);

        SharedPtr<int> integer = ASTEROID_ALLOCATE_SHARED(int, 0);
        SharedPtr<float> single = ASTEROID_ALLOCATE_SHARED_A(float, 16, 0.f);

        return true;
    }

    void WindowsApplication::Finalize()
    {
        DestroyWindow(m_hWnd);

        if (ConsoleVariableManager::Singleton())
        {
            // Unregister all variables, all persistent variables will be saved.
            ConsoleVariableManager::Singleton()->UnregisterAllVariables();
            ConsoleVariableManager::Destroy();
        }

        if (PlayerPrefs::Singleton())
        {
            PlayerPrefs::Singleton()->Save();
            PlayerPrefs::Destroy();
        }

        Debug::Finalize();
    }

    int WindowsApplication::MainMessageLoop()
    {
        MSG msg;
        PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

        while (msg.message != WM_QUIT)
        {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            PerformMainLoop();
        }
        return msg.wParam;
    }

    void WindowsApplication::PerformMainLoop()
    {
    }
}