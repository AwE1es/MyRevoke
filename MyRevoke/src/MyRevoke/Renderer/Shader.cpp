#include "rvpch.h"

#include "Shader.h"
#include "Renderer.h"

#include "MyRevoke/Platform/OpenGL/OpenGlShader.h"


namespace Revoke {
    Shared<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }
    Shared<Shader> Shader::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RV_CORE_ASSERT(false, "RendererAPI None is not supported"); return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return std::make_shared <OpenGLShader>(path);
            break;
        }
        RV_CORE_ASSERT(false, "RendererAPI is not correct")
            return nullptr;
    }
    void ShaderLibrary::LoadShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        if (!ShaderExist(name))
        {
            Shared<Shader> shader = Shader::Create(vertexSrc, fragmentSrc);
            Add(name, shader);
        }
        RV_CORE_ASSERT("Shader {0} already exists!", name);
    }
    void ShaderLibrary::LoadShader(const std::string& name, const std::string& path)
    {

        if(!ShaderExist(name))
        {
            Shared<Shader> shader = Shader::Create(path);
            Add(name, shader);
        }
        RV_CORE_ASSERT("Shader {0} already exists!", name);
    }
    Shared<Shader> ShaderLibrary::GetShaderByName(const std::string& name)
    {
       RV_CORE_ASSERT(ShaderExist(name), "Shader doesn`t exists!")
       return m_ShaderLib[name];
    }
    inline bool ShaderLibrary::ShaderExist(const std::string& name) const
    {
        return m_ShaderLib.find(name) != m_ShaderLib.end();
    }
    inline void ShaderLibrary::Add(const std::string& name, const Shared<Shader>& shader)
    {
        m_ShaderLib[name] = shader;
    }
}