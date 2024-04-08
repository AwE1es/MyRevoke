#pragma once
#include "MyRevoke/Core/Core.h"

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Revoke
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		Shader(const std::string& path);
		~Shader();

		void Bind() const;
		void UnBind() const;


		void SetUniformMat2(const std::string& name, const glm::mat2& value) { BindUniformMat2(name, value); }
		void SetUniformMat3(const std::string& name, const glm::mat3& value) { BindUniformMat3(name, value); }
		void SetUniformMat4(const std::string& name, const glm::mat4& value) { BindUniformMat4(name, value); }
		void SetUniformVec2(const std::string& name, const glm::vec2& value) { BindUniformVec2(name, value); }
		void SetUniformVec3(const std::string& name, const glm::vec3& value) { BindUniformVec3(name, value); }
		void SetUniformVec4(const std::string& name, const glm::vec4& value) { BindUniformVec4(name, value); }
		void SetUniformIntArr(const std::string name, const int* values, const uint32_t count) { BindUniformIntArr(name, values, count); }
		void SetUniformInt(const std::string name, const int value) { BindUniformInt(name, value); }
		void SetUniformFloat(const std::string name, const float value) { BindUniformFloat(name, value); };



		std::string GetName() const { return m_Name; };

		void BindMaterial(const std::string& name, const glm::vec4& color);

		void BindUniformMat2(const std::string& name, const glm::mat2& value);
		void BindUniformMat3(const std::string& name, const glm::mat3& value);
		void BindUniformMat4(const std::string& name, const glm::mat4& value);
		void BindUniformVec2(const std::string& name, const glm::vec2& value);
		void BindUniformVec3(const std::string& name, const glm::vec3& value);
		void BindUniformVec4(const std::string& name, const glm::vec4& value);
		void BindUniformIntArr(const std::string name, const int* values, const uint32_t count);
		void BindUniformInt(const std::string name, const int value);
		void BindUniformFloat(const std::string name, const float value);

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