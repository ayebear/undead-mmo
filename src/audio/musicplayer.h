#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Audio.hpp>
#include "configfile.h"

/*
This class handles loading and playing sets of music.
The filenames of the music files are stored in a config file.
You can also optionally shuffle the order of the music.

To use this class, you can simply create a MusicPlayer object,
    and load a config file with a list of filenames to music files.
    Then, you can call the start() function with the name of the set
    of music to play in the config file.

Example usage:
#include "musicplayer.h"
MusicPlayer music("music.cfg");
music.start("CoolSet");

Example music.cfg file:
    volume = 70
    shuffle = true
    [CoolSet]
    someTrack = sometrack.ogg
    anotherTrack = sometrack2.ogg

TODO:
    Make the public interface a bit simpler
    Use a thread or callback to get rid of the update function
    Support pausing/replaying/next/back
    Store and allow getting the track names from the config file
    Handle loading filenames from other sources besides config files
    Support audio spatialization
*/
class MusicPlayer
{
    public:
        MusicPlayer(const std::string& = "");
        void loadListFromConfig(const std::string&); // Loads sets of music from a config file
        void start(const std::string&); // Starts playing the specified music set
        void playNext(); // Plays the next song in the set
        void playCurrent(); // Plays the current song in the set
        void update(); // This should be called periodically to continue playing songs
        void stop(); // Stops playing all music
        void setVolume(float); // Sets the volume of all the music
        void setShuffle(bool); // Play the set in string order or randomly

    private:
        bool play(unsigned int); // Plays a music file with the ID in the song list
        bool checkNoMusic(); // Updates the noMusic bool to efficiently check if music is playing
        void nextSongId(); // Gets the ID of the next song to play
        void checkSongId(); // Checks if the current ID is valid, otherwise it resets it
        void shuffleSongs(); // Shuffles the current song list

        typedef std::vector<std::string> SongList; // A single list of music filenames
        std::map<std::string, SongList> songs; // Stores all of the song sets
        unsigned int currentSongId; // The currently playing song
        std::string currentSongSet; // The currently playing set of songs
        std::string lastSong; // The last song played (for better shuffling)
        sf::Music music; // The currently playing music object
        bool shuffle; // Whether to shuffle or play the songs in order
        bool noMusic; // For more efficient updating
        static const cfg::File::ConfigMap defaultOptions;
};

#endif
