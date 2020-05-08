#ifndef BOOK_PLAYER_HPP
#define BOOK_PLAYER_HPP

#include "command.hpp"

#include "entity.hpp"
#include "resourceidentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>


#include <map>

class CommandQueue;

class Player : public Entity  // clw Note：如果不写public默认为私有继承
{
	public:
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			ActionCount
		};


	public:
		//Player();
		Player(sf::Texture& texture);

		void					handleEvent(const sf::Event& event, CommandQueue& commands);
		void					handleRealtimeInput(CommandQueue& commands);

		void					assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key		getAssignedKey(Action action) const;


	private:
		void					initializeActions();
		static bool				isRealtimeAction(Action action);

		virtual unsigned int	getCategory() const;
		virtual void		    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		
		void                    updateCurrent(sf::Time dt);
		void                    updateAnimation(sf::Time dt);

	private:
		std::map<sf::Keyboard::Key, Action>		mKeyBinding;
		std::map<Action, Command>				mActionBinding;

		sf::Sprite			mSprite;
		sf::IntRect         mTextureRect;



		sf::Vector2i 			mFrameSize;   // (x,y) 代表 (col, rol)
		std::size_t 			mCurrentFrame;
		sf::Time 		        mFrameDuration;  //mTimePerFrame 
		sf::Time                mElapsedTime;
};

#endif // BOOK_PLAYER_HPP
