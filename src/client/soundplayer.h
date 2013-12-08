#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <string>
#include <list>
#include <map>
#include <SFML/Audio.hpp>
#include "configfile.h"

/// This class handles loading and playing sound effects
class SoundPlayer
{
    public:
        SoundPlayer();
        SoundPlayer(const std::string&);
        ~SoundPlayer();
        void loadFromConfig(const std::string&);
        bool play(const std::string&);
        void setVolume(float);
        void setMaxSounds(unsigned);

    private:
        std::map<std::string, sf::SoundBuffer> soundBuffers;
        std::vector<sf::Sound> playingSounds;
        float volume;
        static const ConfigFile::Section defaultOptions;
};

#endif
