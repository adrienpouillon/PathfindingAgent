#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameManager.h"
#include "SampleScene.h"
#include "Game/MainScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main() 
{
    GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Scene", 60, sf::Color::Black);
	
	pInstance->LaunchScene<MainScene>();

	return 0;
}

// Binds for the Game

// Utils :
// LEFT | RIGHT | UP | DOWN arrows : Move the camera
// & | é : Zoom In - Zoom Out
// F1 | F2 : Change the Grid Current File (It doesn't save the previous Grid so if you want don't forget !)
// + | - : Add or Remove 1 Row and 1 Col on the current Grid (It deletes all the content of the grid !)
// CTRL + S : Save the current Grid !
// H : Delete all entities in current Grid !
// G : Delete all walls in current Grid !
// F : Show debug path and nodes or path or nothing


// Core :
// A + LEFT Click : Place a Guard on the clicked Cell
// A + MIDDLE Click : Place a Civilian on the clicked Cell
// A + RIGHT Click : Place an Assassin on the clicked Cell
// MIDDLE Click : Place a wall on the clicked Cell
// RIGHT Click : Delete the agent or wall on the clicked Cell
// LEFT Click : if clicked on an agent, it's selected
// When an agent is Selected a pink circle is drawn on it.

// When Selected : 
// LEFT click : If an available cell is Clicked, the selected agent travels to it. 
// P : The agent will turn on or off the Patrol Mode, when in Patrol, the agent will make a round trip forever from his start pos to its final pos. 

// GamePlay :
// Civil moves to random Cell in the grid
// Guard moves only if is receive a order to move, if si on collision with the Assassin, it destroys its
// Assassin moves if a Civil is nearby, it go to end path and go back up the path, if si on collision with the Civil, it destroys its
// 
// If a Agent have path and have a new position to go, the new position added after the old position.