#ifndef MENU_H
#define MENU_H

// This class creates a menu and processes user input
// Based on that input, different game states are run

// I will refactor my aMAZEd code for the menu, by making it completely generic, and eventually put it in here

// Maybe we should make this accept function pointers somehow? So it dereferences them and calls them depending on the menu choice?

class Menu
{
    public:
        Menu();
        void Show();
    private:
};

#endif // MENU_H
