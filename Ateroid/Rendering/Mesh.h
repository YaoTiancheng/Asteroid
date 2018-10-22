#pragma once

#include "Core/Object.h"

namespace ASTEROID_NAMESPACE
{
    struct SubmeshInfo
    {
        uint32_t indicesCount;
        uint32_t indexStart;
        int32_t vertexOffset;
    };


    class Mesh : public Object
    {
    public:
        struct BufferData
        {
            void* sysMem;
            uint32_t bytesCount;
            uint32_t bytesStride;
        };

    public:
        virtual ~Mesh();

        bool Create(const BufferData* verticesData,
            uint32_t verticesDataCount,
            const BufferData* indicesData, 
            const SubmeshInfo* submeshes,
            uint32_t submeshesCount,
            const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, 
            uint32_t descsCount);

        void Destroy();

    private:
        std::vector<ID3D11BufferPtr> m_VertexBuffer;
        ID3D11BufferPtr m_IndexBuffer;
        std::vector<SubmeshInfo> m_SubmeshesInfo;
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputElementDesc;
    };
}
