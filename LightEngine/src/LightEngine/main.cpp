#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameManager.h"
#include "SampleScene.h"
#include "../Game/MainScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main() 
{
    GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1920, 1080, "Test Scene", 60, sf::Color::Black);
	
	pInstance->LaunchScene<MainScene>();

	return 0;
}