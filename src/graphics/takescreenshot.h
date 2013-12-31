// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef TAKESCREENSHOT_H
#define TAKESCREENSHOT_H

#include <string>
#include <SFML/Graphics.hpp>

// This functions captures the current window and saves it to a file
void takeScreenshot(sf::RenderWindow& window, const std::string& dir = "");

#endif
