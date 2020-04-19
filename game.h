#pragma once

#include "resourceholder.hpp"
#include "resourceidentifiers.hpp"
#include "statestack.hpp"
#include "player.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Game
{
public:
	Game();
	void Run();

private:
	void ProcessInput();
	void Update(sf::Time dt);
	void Render();
	void UpdateStatistics(sf::Time dt);

	void RegisterStates();


	static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	TextureHolder	 mTextures;
	FontHolder	     mFonts;
	Player           mPlayer;

	StateStack       mStateStack;


	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
};