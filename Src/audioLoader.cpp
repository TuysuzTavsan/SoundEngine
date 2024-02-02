#include <iostream>
#include <fstream>
#include <audioLoader.h>
#include <utility.h>

wavFile* AudioLoader::Loadwav(const char* path)
{
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);


	if (!file.is_open())
	{
		//error
		return nullptr;
	}

	wavFile* wav = new wavFile();
	
	//Read header info.
	file.read(wav->header.ChunkID, 4);
	file.read(reinterpret_cast<char*>(&wav->header.ChunkSize), 4);
	file.read(wav->header.Format, 4);

	//check the header id format and size
	if (!wav->header.isValid())
	{
		delete wav;
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
	wav->data.Data = new std::int16_t[wav->data.SubChunk2Size];
	file.read(reinterpret_cast<char*>(wav->data.Data), wav->data.SubChunk2Size);


	

	


	file.close();

	//success
	return wav;


}