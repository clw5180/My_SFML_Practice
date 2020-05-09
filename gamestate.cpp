#include "gamestate.hpp"
#include "utility.hpp"  // for centerOrigin()
#include "global.h"     // for FONT_SIZE_LARGE

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

// TODO：mWorld在GameState中，mPlayer在mWorld中
GameState::GameState(StateStack& stack, Context context)  
: State(stack, context)
, mWorld(*context.window)
, mPlayer(*mWorld.getPlayer())  //TODO
, mStatisticsUpdateTime()
{
	sf::Font& font = context.fonts->get(Fonts::Main);

	mPlayerPosX.setFont(font);
	mPlayerPosX.setString("X_PlayerPos: 0000"); // clw TODO: 占位符，这样比如1000.0也能显示出来
	mPlayerPosX.setCharacterSize(FONT_SIZE_MIDDLE);
	//playOption.setLetterSpacing(2);
	topRightOrigin(mPlayerPosX);
	mPlayerPosX.setPosition(sf::Vector2f(context.window->getView().getSize().x - MAP_FONT_BOARDER, MAP_FONT_BOARDER)); //clw note

	mPlayerPosY.setFont(font);
	mPlayerPosY.setString("Y_PlayerPos: 0000");
	mPlayerPosY.setCharacterSize(FONT_SIZE_MIDDLE);
	//playOption.setLetterSpacing(2);
	topRightOrigin(mPlayerPosY);
	mPlayerPosY.setPosition(sf::Vector2f(context.window->getView().getSize().x - MAP_FONT_BOARDER, MAP_FONT_BOARDER) + sf::Vector2f(0.f, 50.f)); //clw note
}

void GameState::draw()
{
	mWorld.draw();

	//一些调试的参数打印在屏幕，如角色坐标等
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mPlayerPosX);
	window.draw(mPlayerPosY);
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);  // 更新场景图SceneGraph 
	mWorld.updateWorldView(dt); //根据角色位置更新视角
	sf::View windowView = this->getContext().window->getView();

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);  // clw modify

	////// 更新场景参数
	mStatisticsUpdateTime += dt;
	if (mStatisticsUpdateTime >= sf::seconds(0.1f))
	{
		mStatisticsUpdateTime -= sf::seconds(0.1f);

		mPlayerPosX.setString("X_PlayerPos: " + std::to_string(int(mPlayer.getPosition().x)));
		mPlayerPosY.setString("Y_PlayerPos: " + std::to_string(int(mPlayer.getPosition().y)));
	}
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