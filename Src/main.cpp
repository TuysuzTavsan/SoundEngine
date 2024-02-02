#include <iostream>
#include <PortAudio/portaudio.h>
#include <string>
#include <audioLoader.h>

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

wavFile* wav1 = nullptr;
wavFile* wav2 = nullptr;

struct SampleBuffer
{
    wavFile* wav1 = nullptr;
    wavFile* wav2 = nullptr;
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

    

    wav1 = AudioLoader::Loadwav("Resources/Scargazer.wav"); // Priestess
    wav2 = AudioLoader::Loadwav("Resources/DoctorWho.wav");
    SampleBuffer buffer;
    buffer.wav1 = wav1;
    buffer.wav2 = wav2;

    PaStream* stream;

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(&stream,
        0,
        2,
        paInt16,
        wav1->format.SampleRate,
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
    Pa_Sleep(160000);

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
        if (index >= data->wav1->data.SubChunk2Size / 2 || index >= data->wav2->data.SubChunk2Size / 2)
        {
            return paComplete;
        }


        *out++ = (data->wav1->data.Data[index] * 0.3) + (data->wav2->data.Data[index] * 0.6f);
        *out++ = (data->wav1->data.Data[index + 1] * 0.3) + (data->wav2->data.Data[index + 1] * 0.6f);
        
        index += 2;
        

    }
 
    //index += framesPerBuffer * 2 * 2;


    

    return paContinue;
}
