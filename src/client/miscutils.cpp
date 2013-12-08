// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "miscutils.h"
#include <string>
#include "paths.h"

void takeScreenshot(sf::RenderWindow& window)
{
    std::string fileName = Paths::screenshotsDir + std::to_string(time(0)) + ".png"; // Generate a filename from the system time
    sf::Image scrShot = window.capture(); // Capture the screen to an image
    scrShot.saveToFile(fileName); // Save the image to the file
}
