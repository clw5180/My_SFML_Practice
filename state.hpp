#ifndef BOOK_STATE_HPP
#define BOOK_STATE_HPP

#include <memory>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "stateidentifiers.hpp"
#include "resourceidentifiers.hpp"


namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	/************************************************************************/
	/* In general, every screen will need to display some text or sprites, draw to the screen,
	/* among other common things. Due to this fact, and to avoid unnecessary memory
	/* wasting by loading the same texture or font to memory in multiple places, we
	/* introduced the State::Context structure. It works as a holder of shared objects
	/* between all states of our game.
	/* Essentially, every state will now have access to the getContext() method, which
	/* itself contains the pointer to the window used to draw its objects and resource
	/* holders such as font and texture managers. (P121)
	/* Ҳ���Ǳ�֤�˸�״̬֮����Ը�����ͬ���������������Լ�ڴ棻
	/* ���һ����Ի�ȡ�����ڵ��ӽǣ��Ա�����Դ����ӽ������������λ�ã����߽�������
	/************************************************************************/
	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts );
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
	};
	//struct Context
	//{
	//	Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player);

	//	sf::RenderWindow*  window;
	//	TextureHolder*     textures;
	//	FontHolder*	       fonts;
	//	Player*	           player;
	//};


public:
	State(StateStack& stack, Context context);
	virtual				~State();

	virtual void		draw() = 0;
	virtual bool		update(sf::Time dt) = 0;
	virtual bool		handleEvent(const sf::Event& event) = 0;


protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;


private:
	StateStack*	 mStack;
	Context		 mContext;
};

#endif // BOOK_STATE_HPP
