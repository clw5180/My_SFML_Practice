#include "state.hpp"
#include "statestack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts)
: window(&window)
, textures(&textures)
, fonts(&fonts)
{
}

State::State(StateStack& stack, Context context)
: mStack(&stack)
, mContext(context)
{
}

State::~State()
{
}

/*
As you can see in the source, the StateStack class provides the pushState()
and popState() functions to let us add and remove states from the active stack.
However, in the middle of an event or update iteration by the stack, it is not possible
to alter the active state stack because it would generate a conflict when adding/
removing objects to a container that is being iterated.
Because of this, those functions don't immediately push or pop states into the stack,
but rather register these actions in a pending action list, so they can be processed
later, when it's safe. （P120）
就是说比如你正在状态1下HandleEvent()，结果发现按下了某些键要触发状态2，这时不能直接去修改statestack，因为
正在iter该statestack中，如果在这个时候增加或者删除元素，那迭代过程就会出错，因此先把“触发状态2”这个状态记录
到Pending List，等iter完成后，再修改。
Then, inside your own state logic code, you call requestStackPush() and
requestStackPop(), allowing the states to alter the stack from within their own
code, without risking the safety of the program, thanks to the delayed processing of
push and pop operations.    自注：详见StateStack::applyPendingChanges()  
*/
void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

// completely empty the active stack.
void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}
