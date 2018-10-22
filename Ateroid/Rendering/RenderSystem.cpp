#include "Precompile.h"
#include "Asteroid.h"
#include "RenderSystem.h"
#include "Util/Debug.h"

namespace ASTEROID_NAMESPACE
{
    RenderSystem* RenderSystem::_Singleton = nullptr;

    RenderSystem* RenderSystem::Create(const DXGI_SWAP_CHAIN_DESC& swapChainDesc)
    {
        IDXGIFactory* pFactory = nullptr;
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
            return nullptr;

        IDXGIAdapter* pAdapter = nullptr;
        if (pFactory->EnumAdapters(0, &pAdapter) == DXGI_ERROR_NOT_FOUND)
        {
            pFactory->Release();
            return false;
        }

        pFactory->Release();

        HRESULT hr = 0;
        D3D_FEATURE_LEVEL featureLevelRequested = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
        D3D_FEATURE_LEVEL featureLevelSupported;
        IDXGISwapChain* pSwapChain = nullptr;
        ID3D11Device* pDevice = nullptr;
        ID3D11DeviceContext* pContext = nullptr;
        if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
            pAdapter,
            D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
            nullptr,
            D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG,
            &featureLevelRequested,
            1,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &pSwapChain,
            &pDevice,
            &featureLevelSupported,
            &pContext
        )))
        {
            return nullptr;
        }

        RenderSystem* instance = new RenderSystem(pSwapChain, pDevice, pContext);
        return instance;
    }

    RenderSystem::RenderSystem(IDXGISwapChain* pSwapChain, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
        : m_SwapChain(pSwapChain), m_Device(pDevice), m_Context(pContext)
    {
        ASTEROID_ASSERT(_Singleton == nullptr, "There is already a RenderSystem singleton created.");
        _Singleton = this;
    }

    RenderSystem::~RenderSystem()
    {
        Finalize();
        _Singleton = nullptr;
    }

    ID3D11BufferPtr RenderSystem::CreateBuffer(const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* initialData)
    {
        ID3D11BufferPtr buffer = nullptr;
        HRESULT hr = m_Device->CreateBuffer(desc, initialData, &buffer);
        return buffer;
    }

    bool RenderSystem::Present()
    {
        return SUCCEEDED(m_SwapChain->Present(0, 0));
    }

    void RenderSystem::Finalize()
    {
        m_SwapChain->Release();
        m_Device->Release();
        m_Context->Release();
    }
}


