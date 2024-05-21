#include "rvpch.h"
/*
#include "MusicBuffer.h"
#include <cstddef>
#include <AL\alext.h>
#include <malloc.h>


namespace Revoke
{

	struct MusicData
	{
		const int NUM_BUFFERS = 3;
		const int BUFFER_SAMPLES = 8192;

		short* Membuf;
		SNDFILE* Sndfile;
		SF_INFO Sfinfo;
		ALenum Format;
	};

	static MusicData s_MusicData;

	std::array<uint32_t, 3> MusicBuffer::CreateMusicBuffers(const char* filename)
	{
		ALuint buffer[3];
		alGenBuffers(3, buffer);

		std::size_t frame_size;
		ALenum err;
		ALuint buffer;
		sf_count_t num_frames;
		ALsizei num_bytes;

		s_MusicData.Sndfile = sf_open(filename, SFM_READ, &s_MusicData.Sfinfo);
		if (s_MusicData.Sndfile)
		{
			throw("could not open provided music file -- check path");
		}

		s_MusicData.Format = AL_NONE;
	
		if (s_MusicData.Sfinfo.channels == 1)
			s_MusicData.Format = AL_FORMAT_MONO16;
		else if (s_MusicData.Sfinfo.channels == 2)
			s_MusicData.Format = AL_FORMAT_STEREO16;
		else if (s_MusicData.Sfinfo.channels == 3)
		{
			if (sf_command(s_MusicData.Sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				s_MusicData.Format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (s_MusicData.Sfinfo.channels == 4)
		{
			if (sf_command(s_MusicData.Sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				s_MusicData.Format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!s_MusicData.Format)
		{
			sf_close(s_MusicData.Sndfile);
			s_MusicData.Sndfile = NULL;
			throw("Unsupported channel count from file");
		}

		frame_size = ((size_t)s_MusicData.BUFFER_SAMPLES * (size_t)s_MusicData.Sfinfo.channels) * sizeof(short);
		s_MusicData.Membuf = static_cast<short*>(malloc(frame_size)); //TODO Dont forget that you have membuf here
	}

	ALuint MusicBuffer::CreateMusicSourse()
	{
		ALuint source;
		alGenSources(1, &source);
		return source;
	}

	void MusicBuffer::Play(const uint32_t& sourseID, std::array<uint32_t, 3>  bufferIDs)
	{

		ALsizei i;

		// clear any al errors
		alGetError();

		alSourceRewind(sourseID);
		alSourcei(sourseID, AL_BUFFER, 0);

		for (i = 0; i < 3; i++)
		{
			/* Get some data to give it to the buffer 
			sf_count_t slen = sf_readf_short(s_MusicData.Sndfile, s_MusicData.Membuf, s_MusicData.BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= s_MusicData.Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufferIDs[i], s_MusicData.Format, s_MusicData.Membuf, (ALsizei)slen, s_MusicData.Sfinfo.samplerate);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error buffering for playback");
		}

		/* Now queue and start playback! 
		alSourceQueueBuffers(sourseID, i, bufferIDs);
		alSourcePlay(sourseID);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error starting playback");
		}



	}

	//void MusicBuffer::Pause()
	//{
	//}

	//void MusicBuffer::Stop()
	//{
	//}

	void MusicBuffer::UpdateBufferStream()
	{
		ALint processed, state;

		// clear error 
		alGetError();
		/* Get relevant source info 
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
		alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error checking music source state");
		}

		/* Unqueue and handle each processed buffer 
		while (processed > 0)
		{
			ALuint bufid;
			sf_count_t slen;

			alSourceUnqueueBuffers(p_Source, 1, &bufid);
			processed--;

			/* Read the next chunk of data, refill the buffer, and queue it
			 * back on the source 
			slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
			if (slen > 0)
			{
				slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen,
					p_Sfinfo.samplerate);
				alSourceQueueBuffers(p_Source, 1, &bufid);
			}
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error buffering music data");
			}
		}

		/* Make sure the source hasn't underrun 
		if (state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			/* If no buffers are queued, playback is finished 
			alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0)
				return;

			alSourcePlay(p_Source);
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error restarting music playback");
			}
		}

	}

	ALint MusicBuffer::getSource()
	{
		return p_Source;
	}

	MusicBuffer::Init(const char* filename)
	{
		

	}

	MusicBuffer::~MusicBuffer()
	{
		alDeleteSources(1, &p_Source);

		if (p_SndFile)
			sf_close(p_SndFile);

		p_SndFile = nullptr;

		free(p_Membuf);

		alDeleteBuffers(NUM_BUFFERS, p_Buffers);

	}
}
*/