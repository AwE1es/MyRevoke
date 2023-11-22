#include "rvpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Revoke {
	Revoke::OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{

		// Create an empty vertex OpenGLShader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex OpenGLShader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex OpenGLShader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the OpenGLShader anymore.
			glDeleteShader(vertexShader);

			RV_CORE_ERROR("{0}", infoLog.data());
			RV_CORE_ASSERT(false, "Vertex OpenGLShader Compilation Failure!");
			return;
		}

		// Create an empty fragment OpenGLShader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment OpenGLShader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment OpenGLShader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the OpenGLShader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			RV_CORE_ERROR("{0}", infoLog.data());
			RV_CORE_ASSERT(false, "Fragment OpenGLShader Compilation Failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			RV_CORE_ASSERT(false, "OpenGLShader Link Failure!");
			RV_CORE_ERROR("{0}", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
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

	GLint OpenGLShader::GetUniformLocation(std::string name) const
	{
		if (m_UniformLocations.find(name) != m_UniformLocations.end())
			return m_UniformLocations[name];

		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocations[name] = location;
		return location;
	}
	
}