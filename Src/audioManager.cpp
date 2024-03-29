#include <audioManager.h>
#include <iostream>

const std::uint32_t AudioManager::m_SAMPLERATE = 48000;
std::vector<AudioData> AudioManager::m_activeAudios;
std::mutex AudioManager::m_mutex2;

void AudioManager::Pa_Log(const PaError& err)
{
	 std::cout << "An error occured while using the portaudio stream\n";
	 std::cout << "Error number: " << err << "\n";
	 std::cout << "Error message: " << Pa_GetErrorText(err) << "\n";
}

bool AudioManager::StartStream()
{
    PaError err = Pa_StartStream(m_stream);
    if (err != paNoError)
    {
        Pa_Log(err);
        return false;
    }
    m_streamFlag = true;
    return true;
}

bool AudioManager::StopStream()
{
    PaError err = Pa_StopStream(m_stream);
    if (err != paNoError)
    {
        Pa_Log(err);
        return false;
    }
    m_streamFlag = false;
    return true;
}

int patestCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    (void)timeInfo; /* Prevent unused variable warnings. */
    (void)statusFlags;
    (void)inputBuffer;

    std::vector<AudioData>& Data = *(std::vector<AudioData>*)(userData);
    std::int16_t* Out = (std::int16_t*)(outputBuffer);

    //For loop in range of needed samples at time.
    for (unsigned long i = 0; i < framesPerBuffer; i++)
    {
        std::int16_t left = 0;
        std::int16_t right = 0;
        std::int16_t tempL;
        std::int16_t tempR;

        
        //Get samples and advance in each AudioData.
        for (auto it = Data.begin(); it != Data.end();)
        {
            if (it->IsOver())
            {
                std::scoped_lock lock(AudioManager::m_mutex2);
                it = Data.erase(it);
                continue;
            }

            memcpy(&tempL, &it->m_audioFile->data.get()[it->m_offset], it->m_audioFile->bitsPerSample / 8);
            left += tempL * it->m_volume * 1 / Data.size();

            //advance
            it->m_offset += it->m_audioFile->bitsPerSample / 8;

            memcpy(&tempR, &it->m_audioFile->data.get()[it->m_offset]
                , it->m_audioFile->bitsPerSample / 8);
            right += tempR * it->m_volume * 1 / Data.size();

            //advance
            it->m_offset += it->m_audioFile->bitsPerSample / 8;

            //advance iterator
            it++;
                

        }

        *Out++ = left;
        *Out++ = right;
    }



    return paContinue;
}


AudioManager::AudioManager()
{
    
}

AudioManager::~AudioManager()
{

}

/*
Initialize portAudio library.
*/
bool AudioManager::Initialize()
{
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        Pa_Log(err);
        return false;
    }

    /* Open an audio output stream. */
    err = Pa_OpenDefaultStream(&m_stream,
        0,
        2,
        paInt16,
        m_SAMPLERATE,
        paFramesPerBufferUnspecified,
        patestCallback,
        &m_activeAudios
    );

    if (err != paNoError)
    {
        Pa_Log(err);
        return false;
    }

    return true;
}

/*
Terminate portAudio library
*/
bool AudioManager::Terminate()
{
    PaError err = Pa_Terminate();
    if (err != paNoError)
    {
        Pa_Log(err);
        return false;
    }
    return true;
}

bool AudioManager::Request(const std::string& audioPath, const AudioAttribute& attribute, const float& volume)
{
    //protect request buffer
    std::scoped_lock lock(m_mutex);
    
    try
    {
        m_requestBuffer.Insert(AudioRequest{ audioPath, attribute, volume });   
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
        return false;
    }
    return true;
}

void AudioManager::Update()
{
    //protect request buffer.
    std::unique_lock lock(m_mutex);

    std::vector<AudioRequest> requests;

    //check if request available.
    while (m_requestBuffer.AnyItem())
    {
        requests.push_back(m_requestBuffer.Pop());
    }

    //unlock request buffer after copying.
    lock.unlock();

    std::unique_lock lock2(m_mutex2);
    //load each request.
    for (int i = 0; i < requests.size(); i++)
    {
        //AudioLoader will throw if audioRequest extension is not available.
        m_activeAudios.push_back(std::move(AudioData(requests[i])));
    }
    lock2.unlock();

    if (m_activeAudios.size() > 0 && !m_streamFlag)
    {
        StartStream();
    }

    if (m_activeAudios.size() == 0 && m_streamFlag)
    {
        StopStream();
    }
        
}