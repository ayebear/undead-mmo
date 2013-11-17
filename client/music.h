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
        void playNext();
        void playCurrent();
        void update();
        void stop();
        void setVolume(float);
        void setShuffle(bool);

    private:
        bool play(unsigned int);
        bool checkNoMusic();
        void nextSongId();
        void checkSongId();
        void shuffleSongs();

        typedef std::vector<std::string> SongList;
        std::map<std::string, SongList> songs;
        unsigned int currentSongId;
        std::string currentSongSet;
        std::string lastSong;
        sf::Music music;
        bool shuffle;
        bool noMusic;
        static const ConfigFile::Section defaultOptions;
};

#endif
