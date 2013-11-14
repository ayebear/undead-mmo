#include "music.h"

music::music(std::vector<std::string> songList)
{
    currentSong.openFromFile(songList[0]);
    currentSong.setLoop(true);
    currentSong.play();
}

void music::startMusic()
{
    currentSong.play();
}

void music::stopMusic()
{
    currentSong.stop();
}

void music::setVolume(float vol)
{
    currentSong.setVolume(vol);
}
