#include "rvpch.h"
#include "AudioRenderer.h"

#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>
#include <AL/alc.h>

#include "MyRevoke/Core/Core.h"

namespace Revoke
{
	struct AudioDeviceData
	{
		ALCdevice* ALCDevice;
		ALCcontext* ALCContext;


	};

	static AudioDeviceData s_AudioDeviceData;


	void AudioRenderer::Init()
	{
		s_AudioDeviceData.ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
		if (!s_AudioDeviceData.ALCDevice)
			RV_CORE_ASSERT(false, "failed to get sound device");

		s_AudioDeviceData.ALCContext = alcCreateContext(s_AudioDeviceData.ALCDevice, nullptr);  // create context
		if (!s_AudioDeviceData.ALCContext)
			RV_CORE_ASSERT(false, "Failed to set sound context");

		if (!alcMakeContextCurrent(s_AudioDeviceData.ALCContext))   // make context current
			RV_CORE_ASSERT(false, "failed to make context current");

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(s_AudioDeviceData.ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(s_AudioDeviceData.ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(s_AudioDeviceData.ALCDevice) != ALC_NO_ERROR)
			name = alcGetString(s_AudioDeviceData.ALCDevice, ALC_DEVICE_SPECIFIER);

		RV_ENGINE_INFO("Opened \"%s\"\n", name);
	}

	void AudioRenderer::Shutdown()
	{
		if (!alcMakeContextCurrent(nullptr))
			RV_CORE_ASSERT(false, "failed to set context to nullptr");

		alcDestroyContext(s_AudioDeviceData.ALCContext);
		if (!s_AudioDeviceData.ALCContext)
			RV_CORE_ASSERT(false, "failed to unset during close");

		if (!alcCloseDevice(s_AudioDeviceData.ALCDevice))
			RV_CORE_ASSERT(false, "failed to close sound device");
	}

	void AudioRenderer::RenderAudio(const uint32_t& soundID)
	{
		alSourcePlay(soundID);
	}

	void AudioRenderer::StopRenderingAudio(const uint32_t& soundID)
	{
		alSourceStop(soundID);
	}


	bool AudioRenderer::IsAudioActive(const uint32_t& sourceID)
	{
		ALint state;
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	ALuint AudioRenderer::CreateSoundBuffer(const char* filename)
	{
		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		sndfile = sf_open(filename, SFM_READ, &sfinfo);
		if (!sndfile)
		{
			RV_ENGINE_ERROR("Failed to open an AudioFile: {0}", filename);
			sf_strerror(sndfile);
			return 0;
		}
		if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
		{
			RV_ENGINE_ERROR("Bad sample count in : {0}", filename);
			sf_close(sndfile);
			return 0;
		}

		format = AL_NONE;
		if (sfinfo.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (sfinfo.channels == 2)
			format = AL_FORMAT_STEREO16;
		else if (sfinfo.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfinfo.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!format)
		{
			RV_ENGINE_ERROR("Unsupported channel count in File: {0}", filename);
			sf_close(sndfile);
			return 0;
		}

		/* Decode the whole audio file to a buffer. */
		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			RV_ENGINE_ERROR("Failed to read samples in File: {0}", filename);
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		/* Buffer the audio data into a new buffer object, then free the data and
		 * close the file.
		 */
		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		/* Check if an error occured, and clean up if so. */
		err = alGetError();
		if (err != AL_NO_ERROR)
		{

			RV_ENGINE_ERROR("OpenAL Error: {0}", alGetString(err));
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		return buffer;
	}

	void AudioRenderer::RemoveSoundBuffer(const ALuint& buffer)
	{
		alDeleteBuffers(1, &buffer);

	}
	ALuint AudioRenderer::CreateSoundSource(const float& pitch, const float& gain, const glm::vec3& position, const glm::vec3& velocity, const bool& loopSound, const ALuint& buffer)
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
	void AudioRenderer::UpdateSoundSource(const uint32_t& sourseID, const float& pitch, const float& gain, const glm::vec3& position, const glm::vec3& velocity, const bool& loopSound, const ALuint& buffer)
	{
		alSourcef(sourseID, AL_PITCH, pitch);
		alSourcef(sourseID, AL_GAIN, gain);
		alSource3f(sourseID, AL_POSITION, position.x, position.y, position.z);
		alSource3f(sourseID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alSourcei(sourseID, AL_LOOPING, loopSound);
		alSourcei(sourseID, AL_BUFFER, buffer);
	}
	void AudioRenderer::RemoveSoundSource(const ALuint& source)
	{
		alDeleteSources(1, &source);
	}
}


