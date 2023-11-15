#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Revoke
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();
		
		void Bind() const;
		void UnBind() const;

		void BindUniformMat4(std::string name, glm::mat4 VPmatrix);
	private:
		uint32_t m_RendererID;
	};
}