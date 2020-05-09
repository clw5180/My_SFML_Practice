#include "gamestate.hpp"
#include "utility.hpp"  // for centerOrigin()
#include "global.h"     // for FONT_SIZE_LARGE

#include <SFML/Graphics/RenderWindow.hpp>

// TODO：mWorld在GameState中，mPlayer在mWorld中
GameState::GameState(StateStack& stack, Context context)  
: State(stack, context)
, mWorld(*context.window)
, mPlayer(*mWorld.getPlayer())  //TODO
{
	sf::Font& font = context.fonts->get(Fonts::Main);

	mPlayerPosX.setFont(font);
	mPlayerPosX.setString(L"X: 0.00");
	mPlayerPosX.setCharacterSize(FONT_SIZE_MIDDLE);
	//playOption.setLetterSpacing(2);
	//centerOrigin(mPlayerPosX);
	topRightOrigin(mPlayerPosX);
	mPlayerPosX.setPosition(context.window->getView().getSize() / 2.f); //clw note

	mPlayerPosY.setFont(font);
	mPlayerPosY.setString(L"Y: 0.00");
	mPlayerPosY.setCharacterSize(FONT_SIZE_MIDDLE);
	//playOption.setLetterSpacing(2);
	//centerOrigin(mPlayerPosY);
	topRightOrigin(mPlayerPosY);
	mPlayerPosY.setPosition(context.window->getView().getSize() / 2.f + sf::Vector2f(0.f, 50.f)); //clw note
}

void GameState::draw()
{
	mWorld.draw();

	//一些调试的参数打印在屏幕，如角色坐标等
	sf::RenderWindow& window = *getContext().window;
	window.draw(mPlayerPosX);
	window.draw(mPlayerPosY);
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);  // 更新场景图SceneGraph 
	mWorld.updateWorldView(dt); //根据角色位置更新视角
	sf::View windowView = this->getContext().window->getView();
	mPlayerPosX.setPosition(windowView.getCenter() + sf::Vector2f(windowView.getSize().x / 2.f - MAP_FONT_BOARDER, -windowView.getSize().y / 2.f + MAP_FONT_BOARDER)); //clw note： 注意前面是getCenter()，才能得到具体位置，而不能是getSize()/2，
	mPlayerPosY.setPosition(windowView.getCenter() + sf::Vector2f(windowView.getSize().x / 2.f - MAP_FONT_BOARDER, -windowView.getSize().y / 2.f + MAP_FONT_BOARDER + MAP_TEXT_DISTANCE)); //因为getSize()是固定值，即View的宽高，一般也就是窗口的宽高

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);  // clw modify

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);  // clw modify

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}