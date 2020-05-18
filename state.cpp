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
later, when it's safe. ��P120��
����˵����������״̬1��HandleEvent()��������ְ�����ĳЩ��Ҫ����״̬2����ʱ����ֱ��ȥ�޸�statestack����Ϊ
����iter��statestack�У���������ʱ�����ӻ���ɾ��Ԫ�أ��ǵ������̾ͻ��������Ȱѡ�����״̬2�����״̬��¼
��Pending List����iter��ɺ����޸ġ�
Then, inside your own state logic code, you call requestStackPush() and
requestStackPop(), allowing the states to alter the stack from within their own
code, without risking the safety of the program, thanks to the delayed processing of
push and pop operations.    ��ע�����StateStack::applyPendingChanges()  
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
