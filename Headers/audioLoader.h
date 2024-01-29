#pragma once
#include <wav.h>

class AudioLoader
{
public:
	static wavFile* Loadwav(const char* path);
	
};