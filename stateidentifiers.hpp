#ifndef BOOK_STATEIDENTIFIERS_HPP
#define BOOK_STATEIDENTIFIERS_HPP


namespace States
{
	enum ID
	{
		None,   
		Title,  // clw note: ����ǰ�滹��VideoState ���� from the trailer(Ԥ��Ƭ) of the game to company brand logos
		Menu,
		Game,   // clw note: States::Game Refers to the GameState Class
		Loading,
		Pause,
		Settings
	};
}

#endif // BOOK_STATEIDENTIFIERS_HPP
