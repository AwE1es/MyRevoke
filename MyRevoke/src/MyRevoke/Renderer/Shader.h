#pragma once

#include <string>
#include "MyRevoke/Core.h"
#include <unordered_map>

namespace Revoke
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual std::string GetName() const = 0;

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