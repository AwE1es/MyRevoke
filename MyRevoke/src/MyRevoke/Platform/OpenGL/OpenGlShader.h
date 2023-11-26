#pragma once
#include "MyRevoke/Renderer/Shader.h"

#include <unordered_map>
#include <string>

#include "glm/glm.hpp"

typedef unsigned int GLenum;
namespace Revoke
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& path);
		~OpenGLShader() override;

		void Bind() const override;
		void UnBind() const override;

		void BindMaterial(const std::string& name, const glm::vec4& color);

		void BindUniformMat2(const std::string& name, const glm::mat2& value);
		void BindUniformMat3(const std::string& name, const glm::mat3& value);
		void BindUniformMat4(const std::string& name, const glm::mat4& value);
							  				  				
		void BindUniformVec2(const std::string& name, const glm::vec2& value);
		void BindUniformVec3(const std::string& name, const glm::vec3& value);
		void BindUniformVec4(const std::string& name, const glm::vec4& value);

		void BindUniformInt(const std::string name, int value);

		int GetUniformLocation(std::string name) const;

	private:

		void InitShader(const std::unordered_map<GLenum, std::string>& shadersSoure);
		std::unordered_map<GLenum, std::string> ProcesShader(const std::string& source);
		GLenum ShaderTypeFromString(const std::string& type);
		std::string ReadShader(const std::string& filepath);
		
		uint32_t m_RendererID;
		mutable std::unordered_map<std::string, int> m_UniformLocations;
	};
}