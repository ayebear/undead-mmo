#include "musicplayer.h"
#include <iostream>
#include <algorithm>

const cfg::File::ConfigMap MusicPlayer::defaultOptions = {
{"", {
    {"volume", cfg::makeOption(70, 0, 100)},
    {"shuffle", cfg::makeOption(true)}
}}};

MusicPlayer::MusicPlayer(const std::string& configPath)
{
    noMusic = true;
    music.setLoop(false);
    if (!configPath.empty())
        loadListFromConfig(configPath);
}

void MusicPlayer::loadListFromConfig(const std::string& configPath)
{
    // Load the music sets from the config file into the map
    songs.clear();
    cfg::File musicConfig(configPath, defaultOptions, cfg::File::Warnings | cfg::File::Errors);
    setVolume(musicConfig("volume").toFloat());
    setShuffle(musicConfig("shuffle").toBool());
    for (auto& arr: musicConfig.getSection("Music"))
    {
        for (auto& option: arr.second)
            songs[arr.first].push_back(option);
    }
}

void MusicPlayer::start(const std::string& songSetName)
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

void MusicPlayer::playNext()
{
    // Plays the next song in the list
    if (!checkNoMusic())
    {
        nextSongId();
        play(currentSongId);
    }
}

void MusicPlayer::playCurrent()
{
    // Plays the song with the current song ID
    if (!checkNoMusic())
    {
        checkSongId();
        play(currentSongId);
    }
}

void MusicPlayer::update()
{
    // Continues playing more music when the current music ends
    if (!noMusic && music.getStatus() != sf::Music::Playing)
        playNext();
}

void MusicPlayer::stop()
{
    music.stop();
}

void MusicPlayer::setVolume(float volume)
{
    music.setVolume(volume);
}

void MusicPlayer::setShuffle(bool setting)
{
    shuffle = setting;
}

bool MusicPlayer::play(unsigned int songId)
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

bool MusicPlayer::checkNoMusic()
{
    noMusic = songs[currentSongSet].empty();
    if (noMusic)
    {
        std::cout << "Error: No more music to play!\n";
        stop();
    }
    return noMusic;
}

void MusicPlayer::nextSongId()
{
    ++currentSongId;
    checkSongId();
}

void MusicPlayer::checkSongId()
{
    if (currentSongId >= songs[currentSongSet].size())
    {
        currentSongId = 0;
        shuffleSongs();
    }
}

void MusicPlayer::shuffleSongs()
{
    if (shuffle)
    {
        auto& songSet = songs[currentSongSet];
        if (songSet.size() > 2)
        {
            std::random_shuffle(songSet.begin(), songSet.end()); // Shuffle the songs
            // If the last song played is the same as the new first song
            if (lastSong == songSet.front())
                std::swap(songSet.front(), songSet[(rand() % (songSet.size() - 1)) + 1]);
                // Make it so the next song will be different
        }
    }
}
