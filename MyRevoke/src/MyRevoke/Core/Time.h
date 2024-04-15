#pragma once

namespace Revoke
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time) {}

		float GetSeconds() const { return m_Time; }
		
		operator double() const { return (double)m_Time; }

		operator float() const { return m_Time; }
	private:
		float m_Time = 0.0f;
	};
}