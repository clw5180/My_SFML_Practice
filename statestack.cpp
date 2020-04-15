#include <cassert>

#include "StateStack.hpp"

StateStack::StateStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories()
{
}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

/************************************************************************/
/* The first state to be drawn is the lowest and oldest on the stack, and only then come
/* the others, in order. This grants that the states are transparent, and you will be able
/* to see the underlying screens. Anyway, if you don't desire to see pixels from the
/* lower states, you can use sf::RectangleShape to draw a colored rectangle over the
/* whole screen, blocking the undesired graphics
/************************************************************************/
void StateStack::draw()
{
	// Draw all active states from bottom to top
	for(State::Ptr& state : mStack)
		state->draw();
}

/************************************************************************/
/* Every polled event is fed to the state stack.Then, internally, 
/* the stack will deliver that event to the active states.
/************************************************************************/
void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;  // This gives the control to the states that may not want to let input flow to other states than itself!
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

/************************************************************************/
/* in the middle of an event or update iteration by the stack, it is not possible
to alter the active state stack because it would generate a conflict when 
adding/removing objects to a container that is being iterated.
Because of this, those functions don't immediately push or pop states into the stack,
but rather register these actions in a pending action list, so they can be processed
later, when it's safe. £¨P120£©
/************************************************************************/
void StateStack::applyPendingChanges()
{
	for(PendingChange change : mPendingList)
	{
		switch (change.action)
		{
			case Push:
				mStack.push_back(createState(change.stateID));
				break;

			case Pop:
				mStack.pop_back();
				break;

			case Clear:
				mStack.clear();
				break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
: action(action)
, stateID(stateID)
{
}
