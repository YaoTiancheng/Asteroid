#include "Precompile.h"
#include "Asteroid.h"
#include "Mesh.h"
#include "Util/Debug.h"
#include "RenderSystem.h"

namespace ASTEROID_NAMESPACE
{
    Mesh::~Mesh()
    {
        Destroy();
    }

    bool Mesh::Create(const BufferData* verticesData,
        uint32_t verticesDataCount,
        const BufferData* indicesData,
        const SubmeshInfo* submeshes,
        uint32_t submeshesCount,
        const D3D11_INPUT_ELEMENT_DESC* inputElementDescs,
        uint32_t descsCount)
    {
        ASTEROID_ASSERT(m_VertexBuffer.size() == 0, "Previous created mesh resource is not destroyed yet.");

        // Create vertex buffers
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA initData;
        initData.SysMemPitch = 0;
        initData.SysMemSlicePitch = 0;
        for (uint32_t iBuffer = 0; iBuffer < verticesDataCount; ++iBuffer)
        {
            const BufferData* vertexData = verticesData + iBuffer;

            bufferDesc.ByteWidth = vertexData->bytesCount;
            bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = vertexData->bytesStride;

            initData.pSysMem = vertexData->sysMem;

            ID3D11BufferPtr buffer = RenderSystem::Singleton()->CreateBuffer(&bufferDesc, &initData);
            if (buffer == nullptr)
            {
                // Release previous created buffers
                m_VertexBuffer.clear();
                return false;
            }

            m_VertexBuffer.push_back(buffer);
        }

        // Create index buffer
        if (indicesData != nullptr)
        {
            bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
            bufferDesc.ByteWidth = indicesData->bytesCount;
            bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = indicesData->bytesStride;

            initData.pSysMem = indicesData->sysMem;

            ID3D11BufferPtr buffer = RenderSystem::Singleton()->CreateBuffer(&bufferDesc, &initData);
            if (buffer == nullptr)
            {
                m_VertexBuffer.clear();
                return false;
            }

            m_IndexBuffer = buffer;
        }

        // Copy the submesh info
        m_SubmeshesInfo.resize(submeshesCount);
        std::memcpy(&m_SubmeshesInfo[0], submeshes, sizeof(SubmeshInfo) * submeshesCount);

        // Copy the input elements descs
        m_InputElementDesc.resize(descsCount);
        std::memcpy(&m_InputElementDesc[0], inputElementDescs, sizeof(D3D11_INPUT_ELEMENT_DESC) * descsCount);

        return true;
    }

    void Mesh::Destroy()
    {
        m_VertexBuffer.clear();
        m_IndexBuffer.Reset();
        m_SubmeshesInfo.clear();
        m_InputElementDesc.clear();
    }
}
