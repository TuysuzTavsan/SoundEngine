#pragma once
#include <cstdint>
#include <string>
#include <memory>

// reference: http://soundfile.sapp.org/doc/WaveFormat/
// reference : https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

/*
Format Information:




*/

#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_ALAW 0x0006
#define WAVE_FORMAT_MULAW 0x0007
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE

struct Riff
{
    char ChunkID[4];            /* Contains "RIFF" in ASCII (0x52494646 big-endian form).*/ 
    std::uint32_t ChunkSize;    /* This is the size of the rest of the chunk
                                following this number.  This is the size of the
                                entire file in bytes minus 8 bytes for the
                                two fields not included in this count:
                                ChunkID and ChunkSize.*/
    char Format[4];             /* Contains "WAVE" in ASCII.*/


    //initialize as zero always.
    Riff()
        :
        ChunkID{0},
        ChunkSize{0},
        Format{0}
    {
        
    }

    //Check if valid file format. This function will check if the header file has "RIFF" and "WAVE" chars.
    bool isValid()
    {
        
        //TODO check if I can compare with the defined RIFF direclty instead of this shit.
        return std::string{ ChunkID, 4 } == "RIFF" || std::string{Format, 4} == "WAVE";
    }



};

struct fmtChunk
{
    char ChunkID[4];                    /*Contains "fmt " (0x666d7420 big-endian form).*/ 
    std::uint32_t ChunkSize;            /*16 for PCM.  This is the size of the
                                         rest of the Subchunk which follows this number.*/
    std::uint16_t AudioFormat;          /*PCM = 1 (i.e. Linear quantization)
                                        Values other than 1 indicate some
                                        form of compression.*/
    std::uint16_t NumChannels;          /*Mono = 1, Stereo = 2, etc.*/ 
    std::uint32_t SampleRate;           /*8000, 44100 etc.*/ 
    std::uint32_t ByteRate;             /*== SampleRate * NumChannels * BitsPerSample/8*/ 
    std::uint16_t BlockAlign;           /* ==    NumChannels * BitsPerSample / 8
                                        The number of bytes for one sample including
                                        all channels. I wonder what happens when
                                        this number isn't an integer?*/
    std::uint16_t BitsPerSample;        /*8 bits = 8 etc.*/ 
    std::uint16_t ExtansionChunkSize;   /*Size of the extension. If PCM then doesn't exist. 0 or 22.*/
    std::uint16_t ValidBitsPerSample;   /*Number of valid bits.*/
    std::uint32_t ChannelMask;          /*Speaker position mask*/
    char SubFormat[16];                 /*GUID, including the data format code*/

    //initialize as zero.
    fmtChunk()
        :
        ChunkID{0},
        ChunkSize{0},
        AudioFormat{0},
        NumChannels{0},
        SampleRate{0},
        ByteRate{0},
        BlockAlign{0},
        BitsPerSample{0},
        ExtansionChunkSize{0},
        ValidBitsPerSample{0},
        ChannelMask{0},
        SubFormat{0}
    {

    }

    //returns true if the format is PCM
    bool isPCM()
    {
        //TODO check if I can compare with the defined RIFF direclty instead of this shit.
        return AudioFormat == WAVE_FORMAT_PCM;
    }

    //returns true if the format is extensible. CAUTION: DO NOT USE THIS WITHOUT READING BYTES PROPERLY.
    bool isExtensible()
    {
        return ExtansionChunkSize != 0;
    }
};

struct dataChunk
{
    char SubChunk2ID[4];                /* Contains the letters "data" (0x64617461 big-endian form).*/ 
    std::uint32_t SubChunk2Size;        /*== NumSamples * NumChannels * BitsPerSample/8
                                        This is the number of bytes in the data.
                                        You can also think of this as the size
                                        of the read of the subchunk following this 
                                        number.*/
    std::unique_ptr<std::byte> Data;    /* The actual sound Data.*/ 

    //initialize as zero.
    dataChunk()
        :
        SubChunk2ID{0},
        SubChunk2Size{0},
        Data{nullptr}
    {

    }

};

struct wavFile
{
    
    Riff header;
    fmtChunk format;
    dataChunk data;

    //initialize
    wavFile()
        :
        header{},
        format{},
        data{}
    {

    }

};