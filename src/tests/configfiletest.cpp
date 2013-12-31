// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

// This file can be used for testing classes and other code.

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "configfile.h"
#include "option.h"

using namespace std;

//void rangeStressTest();
//void rangeTest();
void newRangeTests();
//void rewriteConfigFile();
//void generateConfigFile();
//void testConfigFileClass();
//void testOptionClass();
//void printOption(Option&);

//const ConfigFile::Section someDefaultOptions = {
    //{"rangeTest", Option::create<const string&>("testing", Range::GTE, 5, Range::LTE, 10)},
    //{"rangeTest", Option::create<double>(20.123, Range::GTE, 5.9, Range::LT, 29.9)},
    //{"justTesting", Option::create<const string&>("lolololol")},
    //{"boolTest", Option::create<bool>(true)},
    //{"long decimal", Option::create<const string&>("1.1234567890987654321")}
//};

int main()
{
    //testOptionClass();
    //testConfigFileClass();
    //generateConfigFile();
    newRangeTests();
    //rangeTest();
    //rangeStressTest();
    return 0;
}

/*void rangeStressTest()
{
    cout << "Reading config file...\n";
    ConfigFile test("testing.cfg");
    cout << test["500000"].asInt() << endl;
    typedef unsigned long long ull;
    for (ull x = 0; x < 10000000; x++)
    {
        //Option& option = test[to_string(x)];
        //option.setRange(Range::GTE, 0, Range::LTE, 100);
        //option.set(rand());
        string name = to_string(x);
        while (name.size() < 8)
            name.insert(name.begin(), '0');
        test[name].set(ull(rand()) * ull(rand()));
    }
    cout << "Done!\n";
    string tmp;
    cin >> tmp;
    cout << "Writing out to a file...\n";
    test.writeConfigFile();
    cout << "Done!\n";
    cin >> tmp;
}*/

/*void rangeTest()
{
    Option testOption;
    testOption.set(20.0);
    testOption.setRange(Range::LT, 30);
    cout << testOption.asInt() << endl;
    testOption.set(31);
    cout << testOption.asInt() << endl;
    testOption.set(25);
    cout << testOption.asInt() << endl << endl;

    Option testOption2(testOption);
    cout << testOption2.asInt() << endl;
    testOption2.setRange(Range::GT, 50);
    cout << testOption2.asInt() << endl;
    testOption2.set(100);
    cout << testOption2.asInt() << endl << endl;

    testOption.set(22);
    cout << testOption.asInt() << endl;

    cout << "\n\nCopying option object...\n\n";

    Option copiedOption = testOption;
    cout << copiedOption.asDouble() << endl;
    copiedOption.set<double>(40.0);
    cout << copiedOption.asDouble() << endl;
    copiedOption.set<double>(25.0);
    cout << copiedOption.asDouble() << endl;
}*/

void newRangeTests()
{
    ConfigFile test;
    cout.precision(16);
    //test["rangeTest"].setRange(10);
    test["rangeTest"].set(5);
    test["rangeTest"].set(10.1);
    test["rangeTest"].set("Testing!!!");

    cout << test["rangeTest"].asDouble() << endl;
    cout << test["rangeTest"].asString() << endl;

    Option test2 = makeOption(500, 700);
    cout << test2.set(600) << endl;
    cout << test2.set(1500) << endl;
    cout << test2.asDouble() << endl;

    Option test3("testing...");
    cout << test3.asString() << endl;

    //Option test4 = makeOption("this is test 4");
    //cout << test4.asString() << endl;
}

/*void rewriteConfigFileOld()
{
    ConfigFile test("test.cfg");//, someDefaultOptions);
    cout << "Loaded config file\n";
    //test.getOption("boolTest").set<float>(1.23456789);
    //test.getOption("boolTest").setQuotes(false);
    //cout << "Erase returned " << StringUtils::toString<bool>(test.eraseSection()) << "\n\n";
    //cout << "Bool value = " << test.getOption("boolTest").asBool() << endl;
    cout.precision(16);
    //cout << test.getOption("a very long decimal", "defaults").asDouble() << "\n\n";
    //test.getOption("a very long decimal", "defaults").set<double>(2.123456789012345);
    //cout << test.getOption("a very long decimal", "defaults").asDouble() << "\n\n";

    //cout << test.getOption("rangeTest").asDouble() << endl;
    //test.getOption("rangeTest").set<int>(30);
    //cout << test.getOption("rangeTest").asDouble() << endl;

    cout << test["rangeTest"].asString() << endl;
    //test.getOption("rangeTest").setString("test2");
    //cout << test.getOption("rangeTest").asString() << endl;

    Option& stringTest = test.getOption("stringTest");
    stringTest.setString("this is a test");
    cout << stringTest.asStringWithQuotes() << endl;
    stringTest.setString("123456");
    cout << stringTest.asStringWithQuotes() << endl;

    //cout << "In-memory representation after processing:\n" << test.buildString() << endl;
    test.writeConfigFile();
}*/

/*
void generateConfigFile()
{
    srand(time(nullptr));
    cout << "Opening config file...\n";
    ConfigFile test("test.cfg");
    string name("a");
    string section;
    Option value;
    int randNum;
    cout << "Generating values...\n";
    for (int num = 0; num < 5000; num++)
    {
        name.insert(name.begin() + (rand() % name.size()), char('a' + (rand() % 26)));
        randNum = rand() % 4;
        if (randNum == 0)
            value.set<long>(num);
        else if (randNum == 1)
            value.setString(name);
        else if (randNum == 2)
            value.set<double>(num * 1.01234567);
        else
            value.set<long>(rand() % 2);
        if (rand() % 20 == 10)
            section = name + 's';
        test.getOption(name, section) = value;
    }
    cout << "Writing config file...\n";
    test.writeConfigFile("test2.cfg");
    cout << "Done!\n";
}

void testConfigFileClass()
{
    ConfigFile settings("settings.txt", someDefaultOptions);
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
    maxNumOfZombies.set<int>(999);
    printOption(maxNumOfZombies);
}

void printOption(Option& setting)
{
    cout << "As string: " << setting.asString() << endl;
    cout << "As int: " << setting.asInt() << endl;
    cout << "As double: " << setting.asDouble() << endl;
    cout << "As bool: " << (setting.asBool() ? "true" : "false") << endl;
}
*/
