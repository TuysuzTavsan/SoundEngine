#pragma once
#include <memory>
#include <audioFile.h>
#include <audioRequest.h>
#include <audioLoader.h>

struct AudioData
{
	std::unique_ptr<AudioFile> m_audioFile;
	float m_volume;
	std::uint32_t m_offset;

	AudioData()
		:
		m_audioFile{ nullptr },
		m_volume{ 1 },
		m_offset{ 0 }
	{}

	AudioData(const AudioRequest& req)
		:
		m_audioFile{nullptr},
		m_volume{ req.m_volume },
		m_offset{}
	{
		m_audioFile = std::move(AudioLoader::Load(req.m_path));
	}

	bool IsOver() const
	{
		return (m_offset == m_audioFile->dataSize);
	}
};