// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

using namespace std;

class Config
{
    public:
        Config();
        Config(string);
        void Read(string);
    private:
        map<string,int> settings; // This will need to be set with all of the default values first.
        // Then, it will simply read from the file, and replace any values with them.
        // There will need to be something to store all of these default values, for both the client and the server,
        // but this same code can be used for both.
};

#endif // CONFIG_H
