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


		virtual void SetUniformMat2(const std::string& name, const glm::mat2& value) override { BindUniformMat2(name, value); }
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override { BindUniformMat3(name, value); }
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override { BindUniformMat4(name, value); }
		virtual void SetUniformVec2(const std::string& name, const glm::vec2& value) override { BindUniformVec2(name, value); }
		virtual void SetUniformVec3(const std::string& name, const glm::vec3& value) override { BindUniformVec3(name, value); }
		virtual void SetUniformVec4(const std::string& name, const glm::vec4& value) override { BindUniformVec4(name, value); }
		virtual void SetUniformInt(const std::string name, int value) override { BindUniformInt(name, value); }
		virtual void SetUniformFloat(const std::string name, float value) override { BindUniformFloat(name, value); };



		virtual std::string GetName() const override { return m_Name; };

		void BindMaterial(const std::string& name, const glm::vec4& color);

		void BindUniformMat2(const std::string& name, const glm::mat2& value);
		void BindUniformMat3(const std::string& name, const glm::mat3& value);
		void BindUniformMat4(const std::string& name, const glm::mat4& value);	  				  				
		void BindUniformVec2(const std::string& name, const glm::vec2& value);
		void BindUniformVec3(const std::string& name, const glm::vec3& value);
		void BindUniformVec4(const std::string& name, const glm::vec4& value);
		void BindUniformInt(const std::string name, int value);
		void BindUniformFloat(const std::string name, float value);

		int GetUniformLocation(std::string name) const;

	private:
		void InitShader(const std::unordered_map<GLenum, std::string>& shadersSoure);
		std::unordered_map<GLenum, std::string> ProcesShader(const std::string& source);
		GLenum ShaderTypeFromString(const std::string& type);
		std::string ReadShader(const std::string& filepath);
	private:
		uint32_t m_RendererID;
	private:
		std::string m_Name;
		mutable std::unordered_map<std::string, int> m_UniformLocations;
	};
}