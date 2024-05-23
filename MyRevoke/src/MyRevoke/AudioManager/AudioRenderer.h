#pragma once
#include <AL/al.h>

#include <glm/glm.hpp>


namespace Revoke
{
	class AudioRenderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void RenderAudio(const uint32_t& soundID);
		static void StopRenderingAudio(const uint32_t& soundID);

		static bool IsAudioActive(const uint32_t& sourceID);

		static ALuint CreateSoundBuffer(const char* filename);

		static void RemoveSoundBuffer(const ALuint& buffer);

		static ALuint CreateSoundSource(const float& pitch, const float& gain, const glm::vec3& position, const glm::vec3& velocity, const bool& loopSound, const ALuint& buffer);

		static void UpdateSoundSource(const uint32_t& sourseID, const float& pitch, const float& gain, const glm::vec3& position, const glm::vec3& velocity, const bool& loopSound, const ALuint& buffer);

		static void RemoveSoundSource(const ALuint& source);


	};
}


