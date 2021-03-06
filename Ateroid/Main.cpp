#include "Precompile.h"
#include "Asteroid.h"
#include "WindowsApplication.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    ASTEROID_NAMESPACE::WindowsApplication app(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return app.Run();
}
