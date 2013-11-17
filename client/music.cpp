#include "music.h"
#include <iostream>
#include <algorithm>

const ConfigFile::Section Music::defaultOptions = {
    {"volume", makeOption(70)},
    {"shuffle", makeOption(true)}
};

Music::Music(const std::string& configPath)
{
    noMusic = true;
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
    // Start playing the specified music set
    if (songSetName != currentSongSet)
    {
        currentSongSet = songSetName;
        currentSongId = 0;
        shuffleSongs();
        playCurrent();
    }
}

void Music::playNext()
{
    // Plays the next song in the list
    if (!checkNoMusic())
    {
        nextSongId();
        play(currentSongId);
    }
}

void Music::playCurrent()
{
    // Plays the song with the current song ID
    if (!checkNoMusic())
    {
        checkSongId();
        play(currentSongId);
    }
}

void Music::update()
{
    if (!noMusic && music.getStatus() != sf::Music::Playing)
        playNext();
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

bool Music::play(unsigned int songId)
{
    // Play the song with the specified ID
    bool status = false;
    auto& songSet = songs[currentSongSet];
    // Continue trying to play a song until one is successfully played
    while (!status && !songSet.empty() && songId < songSet.size())
    {
        std::cout << "Playing song " << songId + 1 << "/" << songSet.size() << ": " << songSet[currentSongId] << std::endl;
        if (music.openFromFile(songSet[songId]))
        {
            music.play();
            status = true;
            lastSong = songSet[songId];
        }
        else
            songSet.erase(songSet.begin() + songId); // Don't try playing the song again if it cannot be loaded
    }
    checkNoMusic();
    return status;
}

bool Music::checkNoMusic()
{
    noMusic = songs[currentSongSet].empty();
    if (noMusic)
    {
        std::cout << "Error: No more music to play!\n";
        stop();
    }
    return noMusic;
}

void Music::nextSongId()
{
    ++currentSongId;
    checkSongId();
}

void Music::checkSongId()
{
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
        if (songSet.size() > 2)
        {
            std::random_shuffle(songSet.begin(), songSet.end()); // Shuffle the songs
            if (lastSong == songSet.front()) // If the last song played is the same as the new first song
                std::swap(songSet.front(), songSet[(rand() % (songSet.size() - 1)) + 1]); // Make it so the next song will be different
        }
    }
}
