#include "pch.h"

#include <iostream>

#include "LightEngine/GameManager.h"
#include "LightEngine/SampleScene.h"
#include "Game/MainScene.h"
//#include "Game/MainScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main() 
{
    GameManager* pInstance = GameManager::Get();

	pInstance->InstanceWindow(WINDOW_WIDTH, WINDOW_HEIGHT, L"Test Scene");
	pInstance->LaunchScene<MainScene>();

	return 0;
}

// Binds for the Game

// Utils :
// Z | Q | S | D : Move Cursor
// NUMPAD8 | NUMPAD4 | NUMPAD5 | NUMPAD6 : Move Camera
// NUMPAD7 | NUMPAD9 : Zoom In - Zoom Out
// UP | LEFT | DOWN | RIGHT (Arrows) : Rotate Camera
// 
// F1 | F2 : Change the Grid Current File (It doesn't save the previous Grid so if you want don't forget !)
// + | - : Add or Remove 1 Row and 1 Col on the current Grid (It deletes all the content of the grid !)
// CTRL + S : Save the current Grid !
// G : Delete all walls and holes in the current Grid !
// H : Delete all Agents in the current Grid !


// Core :
// A + LEFT Click : Place an Guard on the clicked Cell
// A + MIDDLE Click : Place a Civil on the clicked Cell
// A + RIGHT Click : Place an Assassin on the clicked Cell
// MIDDLE Click : Place a wall on the clicked Cell
// RIGHT Click : Delete the agent or wall on the clicked Cell
// LEFT Click : if clicked on an agent, it's selected
// When an agent is Selected, its color is inverted !


// When Selected : 
// LEFT click : If an available cell is Clicked, the selected agent travel to it. 
// P : The agent will turn on or off the Patrol Mode, when in Patrol, the agent will make a round trip forever from his start pos to its final pos. 


// GamePlay :
// Civil moves to random Cell in the grid
// Guard moves only if is receive a order to move, if si on collision with the Assassin, it destroys its
// Assassin moves if a Civil is nearby, it go to end path and go back up the path, if si on collision with the Civil, it destroys its
// 
// If a Agent have path and have a new position to go, the new position added after the old position.