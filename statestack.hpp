#ifndef BOOK_STATESTACK_HPP
#define BOOK_STATESTACK_HPP
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

#include "state.hpp"
#include "stateidentifiers.hpp"
#include "resourceidentifiers.hpp"

//namespace sf   // clw note: TODO
//{
//	class Event;
//	class RenderWindow;
//}

class StateStack : private sf::NonCopyable  // clw note：把拷贝构造函数和 = 运算符重载 声明为私有
{
	public:
		enum Action
		{
			Push,
			Pop,
			Clear,
		};


	public:		
		explicit			StateStack(State::Context context);

		template <typename T>
		void				registerState(States::ID stateID);

		void				update(sf::Time dt);
		void				draw();
		void				handleEvent(const sf::Event& event);

		void				pushState(States::ID stateID);
		void				popState();
		void				clearStates();

		bool				isEmpty() const;


	private:
		State::Ptr			createState(States::ID stateID);
		void				applyPendingChanges();


	private:
		struct PendingChange
		{
			explicit			PendingChange(Action action, States::ID stateID = States::None);
			Action				action;
			States::ID			stateID;
		};


	private:
		std::vector<State::Ptr>								mStack;
		std::vector<PendingChange>							mPendingList;
		State::Context										mContext;
		std::map<States::ID, std::function<State::Ptr()>>	mFactories;
};

/*
Initially, we register inside the stack all the states we may use. We do not create all
the state objects from the beginning, since some of them may never exist, therefore
we avoid loading resources of never-used states. Instead, we have factory functions
that create a new state on-demand, represented by std::function. The member
variable StateStack::mFactories maps state IDs to those factory functions. (P117)

A lambda expression acts as a factory for the state: 
It creates an object of the derived class T by passing the stack and context to its constructor. 
This object is wrapped into a unique pointer and returned as a base class pointer. 
The lambda expression is assigned to the corresponding state ID in the map.
This approach has the advantage that the StateStack class need not know the
concrete state classes, we thus keep dependencies low
*/
template <typename T>   // The template parameter T is the derived state class we want to register.
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()  
	{
		return State::Ptr(new T(*this, mContext));
	};
}

#endif // BOOK_STATESTACK_HPP
