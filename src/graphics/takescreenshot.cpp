// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "takescreenshot.h"

// In the future this could take a filename
void takeScreenshot(sf::RenderWindow& window, const std::string& dir)
{
    std::string fileName = dir + std::to_string(time(0)) + ".png"; // Generate a filename from the system time
    sf::Image scrShot = window.capture(); // Capture the screen to an image
    scrShot.saveToFile(fileName); // Save the image to the file
}
