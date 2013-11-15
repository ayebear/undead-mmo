#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <SFML/Audio.hpp>
#include "configfile.h"

//std::vector<std::string> menuSongs = {"data/audio/music/titleScreen.ogg"};

class Music
{
    public:
        Music(const std::string&);
        void startMusic(const std::string&);
        void playNext();
        void play();
        void update();
        void stopMusic();
        void setVolume(float);
    private:
        std::vector<std::string> songList;
        unsigned int currentSongId;
        ConfigFile musicConfig;
        float volume = 70;
        sf::Music currentSong;
        std::string currentSongSet;
};

#endif // MUSIC_H
