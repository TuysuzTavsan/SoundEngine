#pragma once
#include <cstdint>

// reference: http://soundfile.sapp.org/doc/WaveFormat/

typedef std::uint8_t mchar;


struct Riff
{
    // uses big endian form.
    mchar ChunkID[4];       // Contains "RIFF" in ASCII (0x52494646 big-endian form).
    mchar ChunkSize[4];     /* This is the size of the rest of the chunk
                             following this number.  This is the size of the
                             entire file in bytes minus 8 bytes for the
                             two fields not included in this count:
                             ChunkID and ChunkSize.*/
    mchar Format[4];        // Contains "WAVE" in ASCII 
};

struct fmtChunk
{
    mchar ChunkID[4];            // Contains "fmt " (0x666d7420 big-endian form).
    mchar ChunkSize[4];         /*16 for PCM.  This is the size of the
                                 rest of the Subchunk which follows this number.*/
    mchar AudioFormat[2];        /*PCM = 1 (i.e. Linear quantization)
                                 Values other than 1 indicate some
                                 form of compression.*/
    mchar NumChannels[2];       // Mono = 1, Stereo = 2, etc.
    mchar SampleRate[4];        // 8000, 44100 etc.
    mchar ByteRate[4];           // == SampleRate * NumChannels * BitsPerSample/8
    mchar BlockAlign[2];        /* ==    NumChannels * BitsPerSample / 8
                                The number of bytes for one sample including
                                all channels. I wonder what happens when
                                this number isn't an integer?*/
    mchar BitsPerSample[2];      // 8 bits = 8 etc.
    mchar ExtraParamSize[2];    //if PCM then doesn't exist.
    mchar ValidBitsPerSample[2]; //Number of valid bits.
    mchar ChannelMask[4];           //Speaker position mask
    mchar SubFormat[16];            //GUID, including the data format code
};

struct dataChunk
{
    mchar SubChunk2ID[4];       // Contains the letters "data" (0x64617461 big-endian form).
    mchar SubChunk2Size[4];     /*== NumSamples * NumChannels * BitsPerSample/8
                               This is the number of bytes in the data.
                               You can also think of this as the size
                               of the read of the subchunk following this 
                               number.*/
    mchar* Data;                // The actual sound Data.
};

struct wav
{
    
    Riff header;
    fmtChunk format;
    dataChunk data;

};