#pragma once



#include <PortAudio/portaudio.h>
#include <audioFile.h>
#include <audioRequest.h>
#include <audioLoader.h>
#include <audioData.h>
#include <ringBuffer.h>

#include<cstdint>
#include <mutex>
#include <vector>
#include <string>
#include <memory>

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
