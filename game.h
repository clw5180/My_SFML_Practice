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
	static Game* Instance()
	{
		if (s_pGame == NULL)
			s_pGame = new Game();
		return s_pGame;
	}

	void Run();

private:
	Game();
	~Game();

	void ProcessInput();
	void Update(sf::Time dt);
	void Render();
	void UpdateStatistics(sf::Time dt);

	void RegisterStates();


	static const sf::Time TimePerFrame;

	sf::RenderWindow mWindow;
	sf::Image        mIcon;
	TextureHolder	 mTextures;
	FontHolder	     mFonts;
	//Player           mPlayer;

	StateStack       mStateStack;


	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;

	static Game* s_pGame;
};