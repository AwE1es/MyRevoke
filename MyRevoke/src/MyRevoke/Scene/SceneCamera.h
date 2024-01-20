#pragma once
#include "MyRevoke/Renderer/Cmaera.h"

namespace Revoke {

	class SceneCamera : public Camera
	{
	public:
		enum class Projection
		{
			Perspective = 0,
			Orthographic = 1
		};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

		Projection GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(const Projection& projectionType) { m_ProjectionType = projectionType; }

	private:
		void RecalculateProjection();
	private:
		Projection m_ProjectionType = Projection::Orthographic;
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};

}