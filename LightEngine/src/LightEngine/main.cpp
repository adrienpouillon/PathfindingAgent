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