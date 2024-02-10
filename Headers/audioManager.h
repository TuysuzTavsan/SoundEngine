#pragma once


#include <ringBuffer.h>
#include <PortAudio/portaudio.h>
#include <audioFile.h>
#include <audioRequest.h>

#include<cstdint>
#include <mutex>
#include <vector>
#include <string>
#include <memory>

struct AudioData
{
	std::unique_ptr<AudioFile> m_audioFile;
	float m_volume;
	std::uint32_t m_offset;

	AudioData(std::unique_ptr<AudioFile> audioFile, AudioRequest& request)
		:
		m_audioFile{std::move(audioFile)},
		m_volume{ request.m_volume },
		m_offset{ 0 }
	{}

	AudioData()
	:
	m_audioFile{nullptr},
	m_volume{1},
	m_offset{0}
	{}

	AudioData& operator=(AudioData&& other) noexcept
	{
		if (this != &other)
		{
			m_audioFile = std::move(other.m_audioFile);
			m_volume = std::move(other.m_volume);
			m_offset = std::move(other.m_offset);
		}
		return *this;
	}

	AudioData& operator=(const AudioData& other) = delete;
	
	~AudioData() = default;
};

/*
AudioManager is the high level manager of the audio.
Application must have one instance of this manager.
Any thread can safely request to play a sound.
Every audio file name must be unique.
Manager will allow audio play / stop - play as loop - set the volume.
This class is thread safe.
*/

class AudioManager
{
private:

	static const std::uint32_t m_SAMPLERATE;
	RingBuffer<AudioRequest> m_requestBuffer;
	PaStream* m_stream = nullptr;
	std::mutex m_mutex;
	std::vector<AudioData> m_activeAudios;


	/*
	PortAudio generic error log helper function.
	*/
	void Pa_Log(const PaError& err);

	/*
	AudioManager will open stream whenever it has a request.
	*/
	bool StartStream();

	/*
	AudioManager will close stream whenever it is not needed.
	*/
	bool StopStream();

	/*
	PortAudio callback function that will be passed to PortAudio Library.
	This callback function will be called when PortAudio needs audio data to play.
	AudioManager will constantly check for request in ringbuffer.
	*/
	int static patestCallback(const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);

public:

	
	AudioManager();

	~AudioManager();

	/*
	Initialize portAudio library.
	*/
	bool Initialize();

	/*
	Terminate portAudio library
	*/
	bool Terminate();

	/*
	Request to play audio from given path and attributes.
	AudioManager will do its best to play it as soon as possible.
	This function is thread safe and potantially lock.
	*/
	bool Request(const std::string& audioPath, const AudioAttribute& attribute, const float& volume);

	/*
	Manage requests and update buffer data.
	*/
	void Update();

};

const std::uint32_t AudioManager::m_SAMPLERATE = 44100;
