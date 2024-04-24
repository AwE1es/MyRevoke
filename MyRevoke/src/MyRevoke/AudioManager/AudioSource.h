#pragma once
#include <AL\al.h>

#include <glm/glm.hpp>

namespace Revoke
{
	class AudioSource
	{
	public:
		static ALuint AddSound(const float& pitch, const float& gain, const glm::vec3& position, const glm::vec3& velocity, const bool& loopSound, const ALuint& buffer);
		static void ShutDown(const ALuint& source);

	private:
		
	};
}

