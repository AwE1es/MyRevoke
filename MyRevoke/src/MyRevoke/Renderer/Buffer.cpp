#include "rvpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "MyRevoke/Platform/OpenGL/OpenGLBuffer.h"

namespace Revoke {
    Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return std::make_shared <OpenGLVertexBuffer>(vertices, size); 
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
        return nullptr;
    }


    Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return std::make_shared <OpenGLIndexBuffer>(indices, count);
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }
    Shared<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return std::make_shared <OpenGLVertexArray>();
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }

}
