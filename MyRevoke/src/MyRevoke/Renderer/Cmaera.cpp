#include "rvpch.h"
#include "Cmaera.h"
#include "glm/gtc/matrix_transform.hpp"


namespace Revoke
{

	Camera::Camera(float left, float right, float bottom, float top )
	{
		CalculateProjectionMatrix(left, right, bottom, top);
	}


	void Camera::CalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Pos);
		m_ViewMatrix = glm::inverse(transform);
	}

	void Camera::SetCameraPos(glm::vec3 cameraPos)
	{
		m_Pos = cameraPos;
		CalculateViewMatrix();
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::CalculateProjectionMatrix(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
	}
	void Camera::SetFov(float left, float right, float bottom, float top)
	{
		CalculateProjectionMatrix(left, right, bottom, top);
	}
}