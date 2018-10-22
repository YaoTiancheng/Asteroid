#pragma once

namespace ASTEROID_NAMESPACE
{
    class RenderSystem
    {
    public:
        static RenderSystem* Create(const DXGI_SWAP_CHAIN_DESC& swapChainDesc);

        static RenderSystem* Singleton() { return _Singleton; }

        ASTEROID_NON_COPYABLE(RenderSystem)

        ~RenderSystem();

        ID3D11BufferPtr CreateBuffer(const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* initialData);

        

        bool Present();

    private:
        RenderSystem(IDXGISwapChain* pSwapChain, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

        void Finalize();

    public:
        static RenderSystem* _Singleton;

    private:
        IDXGISwapChain* m_SwapChain;
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
    };
}
