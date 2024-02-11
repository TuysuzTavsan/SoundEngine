#pragma once

#include <PortAudio/portaudio.h>
#include <string>

/*
Enum for giving info to the AudioManager.
playOnce, playLoop.
*/
enum AudioAttribute
{
	playOnce,
	playLoop
};

//struct to work with Audio specific requests easily.
struct AudioRequest
{
	std::string m_path; // Path to load audio from.
	AudioAttribute m_attribute; // Audio attribute. i.e. play for once - play as loop.
	float m_volume; // Audio volume from range 0.0f - 1.0f.

	AudioRequest(const std::string& path, const AudioAttribute& attribute, const float& volume = 1.0f)
		:
		m_path{ path },
		m_attribute{ attribute },
		m_volume(volume)
	{

	}

	AudioRequest()
		:
		m_path{},
		m_attribute{},
		m_volume{}
	{

	}

};