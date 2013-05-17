#include "gameengine.h"
#include "gamestate.h"
#include <iostream>

GameEngine::GameEngine()
{
    //ctor
}

GameEngine::~GameEngine()
{
    window.close();
}
void GameEngine::init(int windowWidth, int windowHeight, bool fullscreen, std::string title)
{
    if(fullscreen)
    {
        // Create the window in fullscreen at max resolution
        vidMode = sf::VideoMode::getDesktopMode();
        window.create(vidMode, title, sf::Style::Fullscreen);
    }
    else
    {
        // Create a normal window for now
        vidMode = sf::VideoMode(windowWidth, windowHeight);
        window.create(vidMode, title);
    }

}
void GameEngine::cleanup()
{
	// cleanup the all states
	while ( !states.empty() ) {
		states.back()->cleanup();
		states.pop_back();
	}
	window.close();
}

void GameEngine::changeState(GameState* state)
{
 	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->init(this);
}

void GameEngine::pushState(GameState* state)
{
	// pause current state
	if ( !states.empty() ) {
		states.back()->pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->init(this);
}

void GameEngine::popState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->resume();
	}
}

void GameEngine::handleEvents()
{
    //Let the current gamestate handle the events. Pass in the engine
    states.back()->handleEvents(this);
}

void GameEngine::update()
{
    states.back()->update(this);
}

void GameEngine::draw()
{
    states.back()->draw(this);
}

bool GameEngine::running()
{
    return isRunning;
}

void GameEngine::quit()
{
    isRunning = false;
}
