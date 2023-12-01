#include "rvpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Revoke {
	Revoke::OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		InitShader(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string source = ReadShader(path);
		auto shaderSources = ProcesShader(source);
		InitShader(shaderSources);

		// Extract name from filepath
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
	}

	Revoke::OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Revoke::OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Revoke::OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::BindMaterial(const std::string& name, const glm::vec4& color)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniform4f(location, color.r, color.g, color.b, color.a);
	}

	void OpenGLShader::BindUniformMat2(const std::string& name, const glm::mat2& value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::BindUniformMat3(const std::string& name, const glm::mat3& value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::BindUniformMat4(const std::string& name, const glm::mat4& value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::BindUniformVec2(const std::string& name, const glm::vec2& value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::BindUniformVec3(const std::string& name, const glm::vec3& value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::BindUniformVec4(const std::string& name, const glm::vec4& value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::BindUniformInt(const std::string name, int value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::BindUniformFloat(const std::string name, float value)
	{
		GLuint location = (GLuint)GetUniformLocation(name);
		glUniform1f(location, value);
	}

	GLint OpenGLShader::GetUniformLocation(std::string name) const
	{
		if (m_UniformLocations.find(name) != m_UniformLocations.end())
			return m_UniformLocations[name];

		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocations[name] = location;
		return location;
	}

	void OpenGLShader::InitShader(const std::unordered_map<GLenum, std::string>& shadersSoure)
	{
		
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shadersSoure.size());
		for (auto& kv : shadersSoure)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				RV_CORE_ERROR("{0}", infoLog.data());
				RV_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			RV_CORE_ERROR("{0}", infoLog.data());
			RV_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ProcesShader(const std::string& source)
	{

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			RV_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			RV_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;

	}
	std::string OpenGLShader::ReadShader(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
			;
		}
		else
		{
			RV_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}
	GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		RV_CORE_ERROR("{0} type of shaders is not supported", type);
		return 0;
	}
	
}