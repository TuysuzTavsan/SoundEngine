#pragma once
#include <wav.h>
#include <audioFile.h>

class AudioLoader
{
public:
	static AudioFile* Loadwav(const char* path);
	
};