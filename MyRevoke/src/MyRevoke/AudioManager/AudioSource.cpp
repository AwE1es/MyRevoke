#include "rvpch.h"
#include "AudioSource.h"



namespace Revoke
{
	ALuint AudioSource::AddSound(const float &pitch, const float &gain, const glm::vec3 &position, const glm::vec3& velocity, const bool &loopSound, const ALuint &buffer)
	{
		ALuint source;
		alGenSources(1, &source);
		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_GAIN, gain);
		alSource3f(source, AL_POSITION, position.x, position.y, position.z);
		alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alSourcei(source, AL_LOOPING, loopSound);
		alSourcei(source, AL_BUFFER, buffer);
		return source;
	}
	void AudioSource::ShutDown(const ALuint &source)
	{
		alDeleteSources(1, &source);
	}

}