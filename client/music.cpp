#include "music.h"

Music::Music(const std::string& configPath):
    musicConfig(configPath)
{
    currentSong.setLoop(true);
}

void Music::startMusic(const std::string& songSetName)
{
    if (songSetName != currentSongSet)
    {
        auto& songSection = musicConfig.getSection(songSetName);
        songList.clear();
        for(auto& songFilePath: songSection)
            songList.push_back(songFilePath.second.asString());
    }
}

void Music::play()
{
    currentSong.openFromFile(songList[currentSongId]);
    currentSong.play();
}

void Music::update()
{
    if (currentSong.getStatus() != sf::Music::Playing)
        playNext();
}

void Music::playNext()
{
    currentSongId++;
    if(currentSongId >= songList.size())
        currentSongId=0;
    play();
}

void Music::stopMusic()
{
    currentSong.stop();
}

void Music::setVolume(float vol)
{
    currentSong.setVolume(vol);
}
