#include "AudioSystem.h"

#include "utils/Logger.h"

int AudioSystem::init()
{
    log::info("Initializing AudioSystem...");

    if (m_deviceId != 0)
    {
        log::error("AudioSystem already initialized!");
        return 1;
    }

    m_wantSpec = new SDL_AudioSpec();
    m_haveSpec = new SDL_AudioSpec();

    m_wantSpec->freq = 48000;
    m_wantSpec->format = AUDIO_F32;
    m_wantSpec->channels = 2;
    m_wantSpec->samples = 512;
    m_wantSpec->userdata = m_userData;
    m_wantSpec->callback = m_callback;

    m_deviceId = SDL_OpenAudioDevice(nullptr, 0, m_wantSpec, m_haveSpec, 0);
    if (m_deviceId == 0)
    {
        log::error("Failed to open audio device: %s", SDL_GetError());
        return 1;
    }

    log::info("AudioSystem initialized with spec (Sample Rate: %d, Buffer Size: %d, Channels: %d)", m_haveSpec->freq, m_haveSpec->samples, m_haveSpec->channels);

    delete m_wantSpec;

    return 0;
}

int AudioSystem::start()
{
    if (m_deviceId == 0)
    {
        log::error("AudioSystem not initialized!");
        return 1;
    }

    if (m_callback == nullptr)
    {
        log::error("AudioSystem callback is null!");
        return 1;
    }

    log::info("Starting AudioSystem playback...");
    SDL_PauseAudioDevice(m_deviceId, 0);

    return 0;
}

void AudioSystem::stop()
{
    if (m_deviceId == 0)
        return;

    log::info("Stopping AudioSystem playback...");
    SDL_PauseAudioDevice(m_deviceId, 1);
}

void AudioSystem::setCallback(void (*callback)(void *, uint8_t *, int))
{
    m_callback = callback;
}

AudioSystem::~AudioSystem()
{
    if (m_deviceId != 0)
    {
        SDL_CloseAudioDevice(m_deviceId);
    }

    delete m_haveSpec;
}
