#include "rvpch.h"
#include "FrameBuffers.h"

#include "MyRevoke/Renderer/Renderer.h"
#include "MyRevoke/Platform/OpenGL/OpenGLFrameBuffers.h"

namespace Revoke
{
	
    Shared<FrameBuffers> FrameBuffers::Create(const FrameBufferStats& stats)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return std::make_shared <OpenGLFrameBuffers>(stats);
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }

};
