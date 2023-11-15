#pragma once

namespace Revoke
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{

		}

		inline float GetSeconds() const { return m_Time; }
		inline float GetMiliSeconds() const { return m_Time / 10000.0f; }
	private:
		float m_Time;
	};
}