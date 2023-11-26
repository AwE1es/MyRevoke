#include "rvpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "MyRevoke/Platform/OpenGL/OpenGlShader.h"


namespace Revoke {
    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return new OpenGLShader(vertexSrc, fragmentSrc);
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }
    Shader* Shader::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return new OpenGLShader(path);
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }
}