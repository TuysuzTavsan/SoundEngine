#pragma once

#include <cstdint>
#include <memory>


/*
Common audio file class.
AudioLoader will load files from given path depending on their .extensions.
After loading is done and successful AudioLoader will convert its type to AudioFile.
This common type will make library standard even using different types of audio files.
*/
struct AudioFile {
	std::uint16_t numChannels; /* Number of channels. Mono = 1, Stereo = 2...*/
	std::uint32_t sampleRate; /* Number of samples per second that are taken. 8000, 44100...*/
	std::uint16_t bitsPerSample; /* Number of bits per sample. See SampleType. */
	std::uint32_t dataSize; /* data size in bytes.*/
	std::unique_ptr<std::byte> data; // owns data read from file

	// Initialize as 0.
	AudioFile()
		:
		numChannels{0},
		sampleRate{0},
		bitsPerSample{0},
		dataSize{0},
		data{}
	{

	}

	AudioFile(const AudioFile&) = delete;

	AudioFile(const std::uint16_t& _numChannels,
		const std::uint32_t& _sampleRate,
		const std::uint16_t& _bitsPerSample,
		const std::uint32_t& _dataSize,
		std::unique_ptr<std::byte>& _data)
		:
		numChannels{_numChannels},
		sampleRate{_sampleRate},
		bitsPerSample{_bitsPerSample},
		dataSize{_dataSize},
		data{nullptr}
	{
		data = std::move(_data);
	}

	AudioFile& operator= (AudioFile&& other) noexcept
	{
		if (this != &other)
		{
			numChannels = std::move(other.numChannels);
			sampleRate = std::move(other.sampleRate);
			bitsPerSample = std::move(other.bitsPerSample);
			dataSize = std::move(other.dataSize);
			data = std::move(other.data);
		}
		return *this;
	}
};