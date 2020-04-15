#ifndef BOOK_STATEIDENTIFIERS_HPP
#define BOOK_STATEIDENTIFIERS_HPP


namespace States
{
	enum ID
	{
		None,   
		Title,  // clw note: 往往前面还有VideoState —— from the trailer(预告片) of the game to company brand logos
		Menu,
		Game,   // clw note: States::Game Refers to the GameState Class
		Loading,
		Pause,
		Settings
	};
}

#endif // BOOK_STATEIDENTIFIERS_HPP
