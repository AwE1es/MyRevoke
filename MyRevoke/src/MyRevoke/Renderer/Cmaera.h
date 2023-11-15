#pragma once

#include "glm/glm.hpp"

namespace Revoke
{

	class Camera
	{
	public:
		Camera(float left, float right, float bottom, float top);

		void SetCameraPropeties(glm::vec3 camPos);
		void SetFov(float left, float right, float bottom, float top);

		inline glm::mat4 GetPVMatrix() const { return m_ProjectionViewMatrix; }
		inline glm::mat4 GetViewMatrix() const { return  m_ViewMatrix; }
		inline glm::mat4 GetProjectionMatrix() const { return  m_ProjectionMatrix; }
		
	private:
		void CalculateViewMatrix();
		void CalculateProjectionMatrix(float left, float right, float bottom, float top);
	private:
		glm::vec3 m_Pos;
		glm::vec3 m_Front;
		glm::vec3 m_Up;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;

	};
}
