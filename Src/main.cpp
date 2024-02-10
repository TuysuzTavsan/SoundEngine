#include <iostream>
#include <PortAudio/portaudio.h>
#include <string>
#include <audioLoader.h>
#include <audioFile.h>

void Pa_Log(const PaError& err)
{
    printf("An error occured while using the portaudio stream\n");
    printf("Error number: %d\n", err);
    printf("Error message: %s\n", Pa_GetErrorText(err));
}

static int patestCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData);

AudioFile* wav1 = nullptr;
AudioFile* wav2 = nullptr;

struct SampleBuffer
{
    AudioFile* wav1 = nullptr;
    AudioFile* wav2 = nullptr;
};
    
int main()
{
    std::cout << "Hello World!\n";
    PaError err;

    err = Pa_Initialize();
    if (err != paNoError)
    {
        Pa_Log(err);
        return -1;
    }

    PaDeviceIndex deviceIndex = Pa_GetDeviceCount();

    int count = Pa_GetDeviceCount();
    for (int i = 0; i < count; i++)
    {
        PaDeviceInfo info  = *Pa_GetDeviceInfo(i);
        printf("Device \tID#%d \tname# %s\n", i, info.name);
        PaHostApiInfo apiInfo = *Pa_GetHostApiInfo(info.hostApi);
        

    }

    PaDeviceIndex defaultDevice = Pa_GetDefaultOutputDevice();
    printf("Default device index is %d\n", defaultDevice);

    

    std::unique_ptr<AudioFile> wav1 = AudioLoader::Load("Resources/DoctorWho.wav"); // Priestess
    std::unique_ptr<AudioFile> wav2 = AudioLoader::Load("Resources/Priestess.wav");
    SampleBuffer buffer;
    buffer.wav1 = wav1.get();
    buffer.wav2 = wav2.get();

    PaStream* stream;

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(&stream,
        0,
        2,
        paInt16,
        wav1->sampleRate,
        paFramesPerBufferUnspecified,
        patestCallback,
        &buffer
    );
    if (err != paNoError)
    {
        Pa_Log(err);
        Pa_Terminate();
        return -1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError)
    {
        Pa_Log(err);
        Pa_Terminate();
        return -1;
    }

    /* Sleep for 3 seconds. */
    Pa_Sleep(160000000);

    err = Pa_StopStream(stream);
    if (err != paNoError)
    {
        Pa_Log(err);
        Pa_Terminate();
        return -1;
    }
    
    err = Pa_CloseStream(stream);
    if (err != paNoError)
    {
        Pa_Log(err);
        Pa_Terminate();
        return -1;
    }


    err = Pa_Terminate();
    if (err != paNoError)
    {
        Pa_Log(err);
        return -1;
    }


    return 0;
}

unsigned long index = 0;

static int patestCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    SampleBuffer* data = (SampleBuffer*)userData;
    std::int16_t* out = (std::int16_t*)outputBuffer;

    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    for (unsigned long i = 0; i < framesPerBuffer; i++)
    {
        if (index >= data->wav1->dataSize / 2 || index >= data->wav2->dataSize / 2)
        {
            index = 0;
        }

        std::int16_t tempData[4];
        std::memcpy(&tempData[0], data->wav1->data.get() + index * 2, data->wav1->bitsPerSample / 8);
        std::memcpy(&tempData[1], data->wav1->data.get() + index * 2, data->wav1->bitsPerSample / 8);
        std::memcpy(&tempData[2], data->wav2->data.get() + index * 2, data->wav2->bitsPerSample / 8);
        std::memcpy(&tempData[3], data->wav2->data.get() + index * 2, data->wav2->bitsPerSample / 8);

        //std::memcpy(out++, data->wav1->data.get() + index * 2, data->wav1->bitsPerSample / 8);
        //std::memcpy(out++, data->wav1->data.get() + index * 2, data->wav1->bitsPerSample / 8);

        //*out++ = (data->wav1->data.get()[index] * 0.0) + (data->wav2->data.get()[index] * 0.6f);
        //*out++ = (data->wav1->data.get()[index + 1] * 0.0) + (data->wav2->data.get()[index + 1] * 0.6f);
        
        *out++ = tempData[0] * 0.3 + tempData[2] * 0.4;
        *out++ = tempData[1] * 0.3 + tempData[3] * 0.4;

        index += 2;
        

    }
 
    //index += framesPerBuffer * 2 * 2;


    

    return paContinue;
}
