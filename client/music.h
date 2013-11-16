#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Audio.hpp>
#include "configfile.h"

class Music
{
    public:
        Music(const std::string&);
        void loadListFromConfig(const std::string&);
        void start(const std::string&);
        bool play();
        bool playNext();
        void update();
        void stop();
        void setVolume(float);
        void setShuffle(bool);

    private:
        void updateSongId();
        void shuffleSongs();

        typedef std::vector<std::string> SongList;
        std::map<std::string, SongList> songs;
        unsigned int currentSongId;
        std::string currentSongSet;
        sf::Music music;
        bool shuffle;
        static const ConfigFile::Section defaultOptions;
};

#endif
