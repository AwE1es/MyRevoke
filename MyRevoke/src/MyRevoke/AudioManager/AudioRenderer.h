#pragma once

#include "MyRevoke/Scene/Components.h"

namespace Revoke
{
	class AudioRenderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void Begin();
		static void End();

		static void RenderAudio(SoundComponent& soundComponent);
		static void RenderSong(SongComponent& songComponent);
		
		static bool IsAudioActive(SoundComponent& soundComponent);

	public:

	};
}


