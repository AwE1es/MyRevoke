#include "rvpch.h"
#include "AudioBuffer.h"

#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>

#include <map>

namespace Revoke
{

	static std::vector<ALuint> s_AudioBuffer;


	void AudioBuffer::Init()
	{
		s_AudioBuffer.clear();
	}

	void AudioBuffer::ShutDown()
	{
		alDeleteBuffers(s_AudioBuffer.size(), s_AudioBuffer.data());
		s_AudioBuffer.clear();
	}

	ALuint AudioBuffer::AddSound(const char* filename)
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

		s_AudioBuffer.push_back(buffer);  // add to the list of known buffers

		return buffer;
	}

	bool AudioBuffer::RemoveAudio(const ALuint& buffer)
	{
		alDeleteBuffers(1, &buffer);
		auto it = s_AudioBuffer.begin();
		while (it != s_AudioBuffer.end())
		{
			if (*it == buffer)
			{
			
				it = s_AudioBuffer.erase(it);

				return true;
			}
			else {
				++it;
			}
		}
		return false;
	}



}