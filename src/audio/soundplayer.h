#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Audio.hpp>
#include "configfile.h"

/*
This class handles loading and playing sound effects.

To use this class, you can simply create a SoundPlayer object,
    and load a config file with a list of filenames to sound effects.
    Then, you can call the play() function with the name of the sound
    effect specified in the config file.

Example usage:
#include "soundplayer.h"
SoundPlayer sounds("sounds.cfg");
sounds.play("boom");
sounds.play("pow");

Example sounds.cfg file:
    volume = 100
    maxSounds = 50
    [Sounds]
    boom = boom.ogg
    pow = pow.ogg

Specifying the volume and maxSounds is optional, default values will be used otherwise.
Note that you must put the [Sounds] section header before the sound filenames.

TODO:
    Handle loading filenames from other sources besides config files
    Support audio spatialization
*/
class SoundPlayer
{
    public:
        SoundPlayer();
        SoundPlayer(const std::string&);
        ~SoundPlayer();
        void loadFromConfig(const std::string&); // Loads the sound files listed in a config file
        bool play(const std::string&); // Plays a sound effect with the specified name
        void setVolume(float); // Sets the playback volume of the sound effects
        void setMaxSounds(unsigned); // Sets the maximum number of sounds that can be played at the
            // same time (almost any amount of sound buffers can be loaded into memory at once)

    private:
        std::map<std::string, sf::SoundBuffer> soundBuffers; // Contains the audio samples in memory
        std::vector<sf::Sound> playingSounds; // Contains the currently playing sound instances
        float volume;
        static const ConfigFile::Section defaultOptions;
};

#endif
