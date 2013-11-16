#include "music.h"
#include <iostream>
#include <random>
#include <chrono>

const ConfigFile::Section Music::defaultOptions = {
    {"volume", makeOption(70)},
    {"shuffle", makeOption(true)}
};

Music::Music(const std::string& configPath)
{
    music.setLoop(false);
    loadListFromConfig(configPath);
}

void Music::loadListFromConfig(const std::string& configPath)
{
    songs.clear();
    ConfigFile musicConfig(configPath, defaultOptions, "", true);
    setVolume(musicConfig["volume"].asFloat());
    setShuffle(musicConfig["shuffle"].asBool());
    for (auto& section: musicConfig)
        for (auto& option: section.second)
            if (!section.first.empty())
                songs[section.first].push_back(option.second.asString());

}

void Music::start(const std::string& songSetName)
{
    if (songSetName != currentSongSet)
    {
        currentSongSet = songSetName;
        currentSongId = 0;
        shuffleSongs();
        play();
    }
}

bool Music::play()
{
    bool status = false;
    auto& songSet = songs[currentSongSet];
    if (!songSet.empty() && currentSongId < songSet.size())
    {
        std::cout << "Playing song " << currentSongId << ": " << songSet[currentSongId] << std::endl;
        if (music.openFromFile(songSet[currentSongId]))
        {
            music.play();
            status = true;
        }
        else
        {
            songSet.erase(songSet.begin() + currentSongId); // Don't try playing the song again if it cannot be loaded
            if (currentSongId > 0)
                --currentSongId;
            updateSongId();
        }
    }
    return status;
}

bool Music::playNext()
{
    bool status = true;
    if (!songs[currentSongSet].empty())
    {
        updateSongId();
        status = play();
    }
    return status;
}

void Music::update()
{
    if (music.getStatus() != sf::Music::Playing)
        while (!playNext());
}

void Music::stop()
{
    music.stop();
}

void Music::setVolume(float volume)
{
    music.setVolume(volume);
}

void Music::setShuffle(bool setting)
{
    shuffle = setting;
}

void Music::updateSongId()
{
    ++currentSongId;
    if (currentSongId >= songs[currentSongSet].size())
    {
        currentSongId = 0;
        shuffleSongs();
    }
}

void Music::shuffleSongs()
{
    if (shuffle)
    {
        auto& songSet = songs[currentSongSet];
        std::shuffle(songSet.begin(), songSet.end(), std::default_random_engine(time(nullptr)));
    }
}
