All of the documentation on the game can go in this folder eventually. We could also have some on the wiki.

What is contained in the directories:

Main programs:
client - The game
server - The game server
masterserver - Game server list + account database server

All of the folders and what they contain:
client
    The code for the actual game that people can play.
data
    Resources that the client loads, such as textures and audio.
doc
    Documentation on everything.
lib
    Any libraries that the code depends on.
masterserver
    A separate server program which manages a master account database and the list of game servers.
other
    Any generic pieces of code that we write and depend on. Similar to lib except this actually gets compiled with everything.
projects
    IDE projects that are properly setup to point to the libraries and code for different operating systems.
    These use relative paths and should be placed in the root of the git repository folder.
server
    The game server program, which multiple clients can connect to and play the game.
shared
    Code that is used by multiple programs like the client and server, but is specific to them. Generic code should go in the "other" folder.
