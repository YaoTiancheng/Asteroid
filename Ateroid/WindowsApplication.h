#pragma once

namespace ASTEROID_NAMESPACE
{
    /**
     *  Root caller on windows platform.
     *  Engine initialize/finalize, window management, runs message loop & engine loop, etc.
     */
    class WindowsApplication
    {
    public:
        ASTEROID_NON_COPYABLE(WindowsApplication)

        static WindowsApplication* Singleton() { return _Singleton; }

        WindowsApplication(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int cmdShow);
        ~WindowsApplication();

        int Run();

        LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    public:
        static WindowsApplication* _Singleton;

    private:
        bool Initialize();
        void Finalize();
        int MainMessageLoop();
        void PerformMainLoop();

    private:
        HINSTANCE   m_hInstance;
        int         m_CmdShow;
        HWND        m_hWnd;
    };
}