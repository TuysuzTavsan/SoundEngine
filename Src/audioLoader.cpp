#include <iostream>
#include <fstream>
#include <audioLoader.h>
#include <audioFile.h>
#include <utility.h>

/*
Function will load and return a pointer to the generic AudioFile struct.
if loading fails for any reason it will return nullptr.
*/
AudioFile* AudioLoader::Loadwav(const char* path)
{
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);


	if (!file.is_open())
	{
		//error
		return nullptr;
	}

	std::unique_ptr<wavFile> wav(new wavFile);
	
	//Read header info.
	file.read(wav->header.ChunkID, 4);
	file.read(reinterpret_cast<char*>(&wav->header.ChunkSize), 4);
	file.read(wav->header.Format, 4);

	//check the header id format and size
	if (!wav->header.isValid())
	{
		//error
		return nullptr;
	}

	//Read format chunk info also known as fmt (format?)
	file.read(wav->format.ChunkID, 4);
	file.read(reinterpret_cast<char*>(&wav->format.ChunkSize), 4);
	file.read(reinterpret_cast<char*>(&wav->format.AudioFormat), 2);
	file.read(reinterpret_cast<char*>(&wav->format.NumChannels), 2);
	file.read(reinterpret_cast<char*>(&wav->format.SampleRate), 4);
	file.read(reinterpret_cast<char*>(&wav->format.ByteRate), 4);
	file.read(reinterpret_cast<char*>(&wav->format.BlockAlign	), 2);
	file.read(reinterpret_cast<char*>(&wav->format.BitsPerSample), 2);
	

	//Check if extansionChunkSize exist.
	if (!wav->format.isPCM())
	{
		//Read additional information
		//TODO: IMPLEMENT
		file.read(reinterpret_cast<char*>(&wav->format.ExtansionChunkSize), 2);
	}

	//TODO CHECK FOR VALID FORMAT CHUNK

	//FIND "DATA" string literal
	unsigned int dataOffSet = 0;

findData:
	file.read(wav->data.SubChunk2ID, 4);
	dataOffSet ++;
	if (std::string{ wav->data.SubChunk2ID, 4 } != "data")
	{
		file.seekg(-3, std::ios::cur);
		goto findData;
	}


	//Read Data chunk
	//file.read(wav->data.SubChunk2ID, 4);
	file.read(reinterpret_cast<char*>(&wav->data.SubChunk2Size), 4);
	
	//allocate memory and read
	wav->data.Data = std::unique_ptr<std::byte>(new std::byte[wav->data.SubChunk2Size]);
	file.read(reinterpret_cast<char*>(wav->data.Data.get()), wav->data.SubChunk2Size);

	// Reading is over close the file.
	file.close();

	// load wav file info to the generic AudioFile.
	AudioFile* audio = new AudioFile(wav->format.NumChannels,
		wav->format.SampleRate,
		wav->format.BitsPerSample,
		wav->data.SubChunk2Size,
		wav->data.Data);

	return audio;

}