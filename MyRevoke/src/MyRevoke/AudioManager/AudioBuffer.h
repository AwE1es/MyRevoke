#pragma once
#include <AL\al.h>

#include <vector>

#include "MyRevoke/Core/Core.h"

namespace Revoke
{
	class AudioBuffer
	{
	public:
		static void Init();
		static void ShutDown();

		static ALuint AddSound(const char* filename);
		static bool RemoveAudio(const ALuint& buffer);
	};
}

