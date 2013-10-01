#include "miscutils.h"
#include <string>

void takeScreenshot(sf::RenderWindow& window)
{
    std::string fileName = "data/screenshots/" + std::to_string(time(0)) + ".png"; // Generate a filename from the system time
    sf::Image scrShot = window.capture(); // Capture the screen to an image
    scrShot.saveToFile(fileName); // Save the image to the file
}
