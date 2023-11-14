#include "rvpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "MyRevoke/Platform/OpenGL/OpenGLBuffer.h"

namespace Revoke {
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size); 
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
        return nullptr;
    }


    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, size);
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLVertexArrey();
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }

}
