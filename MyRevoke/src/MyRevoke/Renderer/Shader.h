#pragma once

#include "MyRevoke/Core/Core.h"

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

namespace Revoke
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual std::string GetName() const = 0;

		virtual void SetUniformMat2(const std::string& name, const glm::mat2& value) = 0;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetUniformVec2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetUniformVec3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniformVec4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetUniformInt(const std::string name, int value) = 0;
		virtual void SetUniformFloat(const std::string name, float value) = 0;

		static Shared<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
		static Shared<Shader> Create(const std::string& path);
	};

	class ShaderLibrary
	{
	public:
		ShaderLibrary(){}
		void LoadShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		void LoadShader(const std::string& name, const std::string& path);

		Shared<Shader> GetShaderByName(const std::string& name);

		inline bool ShaderExist(const std::string& name) const;
	private:
		inline void Add(const std::string& name, const Shared<Shader>& shader);
	private:
		std::unordered_map<std::string, Shared<Shader>> m_ShaderLib;
	};

}