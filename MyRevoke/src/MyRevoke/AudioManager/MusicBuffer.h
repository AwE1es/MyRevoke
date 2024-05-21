
/*
#pragma once
#include <AL\al.h>
#include <sndfile.h>
#include <array>

namespace Revoke
{
	class MusicBuffer
	{
	public:
		static void Play(const uint32_t& sourseID, std::array<uint32_t, 3> bufferIDs);
		//void Pause();
		//void Stop();

		static std::array<uint32_t, 3> CreateMusicBuffers(const char* filename);

		static ALuint CreateMusicSourse();
	

		void UpdateBufferStream();

		ALint getSource();

		MusicBuffer(const char* filename);
		~MusicBuffer();

	};
}

*/