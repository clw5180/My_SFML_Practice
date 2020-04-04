#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

class Game
{
public:
	Game();
	void Run();

private:
	void ProcessInput();
	void Render();
	void UpdateStatistics(sf::Time dt);

	static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	FontHolder				mFonts;

	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
};