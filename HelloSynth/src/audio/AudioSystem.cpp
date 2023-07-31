#include "AudioSystem.h"

#include "utils/Logger.h"

int AudioSystem::init()
{
    log::info("Initializing AudioSystem...\n");

    m_wantSpec = new SDL_AudioSpec();
    m_haveSpec = new SDL_AudioSpec();

    m_wantSpec->freq = 48000;
    m_wantSpec->format = AUDIO_F32;
    m_wantSpec->channels = 2;
    m_wantSpec->samples = 512;
    m_wantSpec->userdata = m_userData;
    m_wantSpec->callback = m_callback;

    m_deviceId = SDL_OpenAudioDevice(nullptr, 0, m_wantSpec, m_haveSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (m_deviceId == 0)
    {
        log::error("Failed to open audio device: %s\n", SDL_GetError());
        return 1;
    }

    // Log the have spec
    log::info("AudioSystem initialized:");
    log::info("\tSample Rate: %d", m_haveSpec->freq);
    log::info("\tBuffer Size: %d", m_haveSpec->samples);
    log::info("\tFormat: %d", m_haveSpec->format);
    log::info("\tChannels: %d", m_haveSpec->channels);

    delete m_wantSpec;

    return 0;
}

void AudioSystem::start()
{
    if (m_deviceId == 0)
        return;

    if (m_callback == nullptr)
    {
        log::error("AudioSystem callback is null!\n");
        return;
    }

    log::info("Starting AudioSystem...\n");
    SDL_PauseAudioDevice(m_deviceId, 0);
}

void AudioSystem::stop()
{
    if (m_deviceId == 0)
        return;

    log::info("Stopping AudioSystem...\n");
    SDL_PauseAudioDevice(m_deviceId, 1);
}

void AudioSystem::setCallback(void (*callback)(void *, uint8_t *, int))
{
    m_callback = callback;
}