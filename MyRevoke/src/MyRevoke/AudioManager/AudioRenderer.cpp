#include "rvpch.h"
#include "AudioRenderer.h"

#include <AL/alc.h>
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

	void AudioRenderer::Begin()
	{

	}

	void AudioRenderer::End()
	{

	}

	void AudioRenderer::RenderAudio(SoundComponent& soundComponent)
	{
		alSourcePlay(soundComponent.SoundID);
		soundComponent.HasPlayed = true;
	}
	void AudioRenderer::RenderSong(SongComponent& songComponent)
	{
		alSourcePlay(songComponent.SoundID);
		//soundComponent.HasPlayed = true;
	}

	bool AudioRenderer::IsAudioActive(SoundComponent& soundComponent)
	{
		ALint state;
		alGetSourcei(soundComponent.SourceID, AL_SOURCE_STATE, &state);
		RV_EDITOR_TRACE("currently playing sound\n");
		return (state == AL_PLAYING);
	}

}


