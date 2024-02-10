#pragma once
#include <wav.h>
#include <audioFile.h>

/*
AudioLoader is responsible from loading audio files from given path.
If succeeds it will return a pointer to AudioFile
File path extensions will change loading function.
*/

class AudioLoader
{
private:

	static std::unique_ptr<AudioFile> Loadwav(const std::string& path);

public:

	static std::unique_ptr<AudioFile> Load(const std::string& path);
	
};