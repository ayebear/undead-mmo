#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <SFML/Audio.hpp>

const std::vector<std::string> menuSongs = {"data/audio/music/titleScreen.ogg"};

class music
{
    public:
        music(std::vector<std::string> songList);

        void startMusic();
        void stopMusic();
        void setVolume(float);
    protected:
    private:
        float volume = 70;
        sf::Music currentSong;
};

#endif // MUSIC_H
