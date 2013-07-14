// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

// This file can be used for testing classes and other code.

#include <iostream>
#include "configfile.h"
#include "option.h"

using namespace std;

void rewriteConfigFile();
void generateConfigFile();
void testConfigFileClass();
void testOptionClass();
void printOption(Option&);

const ConfigFile::ConfigMap someDefaultOptions = {
    {"testDec", Option("2.718281828459045")},
    {"justTesting", Option("lolololol")},
    {"348572394871294", Option("testing...")},
};

int main()
{
    //testOptionClass();
    //testConfigFileClass();
    //generateConfigFile();
    rewriteConfigFile();
    return 0;
}

void rewriteConfigFile()
{
    ConfigFile test("test.txt");
    cout << "In-memory representation after processing:\n" << test.buildString() << endl;
    test.writeConfigFile();
}

void generateConfigFile()
{
    // WARNING: The following consumes over 5GB of RAM and generates a ~1.6GB file! Run with caution or lower the iterations of the loop!
    srand(time(nullptr));
    ConfigFile::ConfigMap defaults;
    string name("a");
    Option value;
    int randNum;
    for (int num = 0; num < 50000; num++)
    {
        name.insert(name.begin() + (rand() % name.size()), char('a' + (rand() % 26)));
        randNum = rand() % 4;
        if (randNum == 0)
            value.setInt(num);
        else if (randNum == 1)
            value.setString(name);
        else if (randNum == 2)
            value.setDouble(num * 1.01234567);
        else
            value.setBool(rand() % 2);
        defaults[name] = value;
    }
    ConfigFile crazy(defaults);
    crazy.writeConfigFile("crazy.txt");
}

void testConfigFileClass()
{
    ConfigFile settings(someDefaultOptions, "settings.txt");
    cout << settings.buildString() << endl;
    settings.writeConfigFile("settings2.txt");

    cout << "Some interpretation of values:\n";
    string testStr = settings.getOption("testStr").asString();
    int testNum = settings.getOption("testNum").asInt();
    double testDec = settings.getOption("testDec").asDouble();
    bool testBool = settings.getOption("testBool").asBool();
    cout.precision(16);
    cout << testStr << endl;
    cout << testNum << endl;
    cout << testDec << endl;
    cout << testBool << endl;
}

void testOptionClass()
{
    Option maxNumOfZombies;
    maxNumOfZombies.setString("this is false");
    printOption(maxNumOfZombies);
    maxNumOfZombies.setInt(999);
    printOption(maxNumOfZombies);
}

void printOption(Option& setting)
{
    cout << "As string: " << setting.asString() << endl;
    cout << "As int: " << setting.asInt() << endl;
    cout << "As double: " << setting.asDouble() << endl;
    cout << "As bool: " << (setting.asBool() ? "true" : "false") << endl;
}
