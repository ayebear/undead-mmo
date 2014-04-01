#include "soundplayer.h"
#include <iostream>

const cfg::File::ConfigMap SoundPlayer::defaultOptions = {
{"", {
    {"volume", cfg::makeOption(100, 0, 100)},
    {"maxSounds", cfg::makeOption(50, 1, 200)}
}}};

SoundPlayer::SoundPlayer()
{
    setVolume(100);
}

SoundPlayer::SoundPlayer(const std::string& configPath)
{
    loadFromConfig(configPath);
}

SoundPlayer::~SoundPlayer()
{
    playingSounds.clear();
    soundBuffers.clear();
}

void SoundPlayer::loadFromConfig(const std::string& configPath)
{
    // Load the sound files listed in the config file
    soundBuffers.clear();
    cfg::File soundConfig(configPath, defaultOptions, true);
    setVolume(soundConfig("volume").toFloat());
    setMaxSounds(soundConfig("maxSounds").toInt());
    for (auto& option: soundConfig.getSection("Sounds"))
    {
        if (!soundBuffers[option.first].loadFromFile(option.second.toString()))
            soundBuffers.erase(option.first);
    }
}

bool SoundPlayer::play(const std::string& soundName)
{
    bool status = false;
    auto found = soundBuffers.find(soundName);
    if (found != soundBuffers.end())
    {
        // Find an unused sound object to play the sound buffer
        for (auto& sound: playingSounds)
        {
            if (sound.getStatus() != sf::Sound::Playing)
            {
                sound.setBuffer(found->second);
                sound.play();
                status = true;
                break;
            }
        }
    }
    return status;
}

void SoundPlayer::setVolume(float vol)
{
    volume = vol;
}

void SoundPlayer::setMaxSounds(unsigned maxSounds)
{
    playingSounds.resize(maxSounds);
}
