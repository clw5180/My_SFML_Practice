#ifndef BOOK_ENTITY_HPP
#define BOOK_ENTITY_HPP

#include "scenenode.hpp"


class Entity : public SceneNode
{
	public:
		void				setVelocity(sf::Vector2f velocity);
		void				setVelocity(float vx, float vy);
		void				accelerate(sf::Vector2f velocity);
		void				accelerate(float vx, float vy);
		sf::Vector2f		getVelocity() const;


	protected:
		virtual void		updateCurrent(sf::Time dt);

    //private:   // clw note: TODO
	protected:
		sf::Vector2f		mVelocity;
};

#endif // BOOK_ENTITY_HPP
