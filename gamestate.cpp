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
	mPlayerPosX.setString("xPlayer: 00000"); // clw TODO: 占位符，否则从999到1000的时候，如果是右上角对齐则会抖动
	mPlayerPosX.setCharacterSize(FONT_SIZE_MIDDLE);
	//playOption.setLetterSpacing(2);
	mPlayerPosX.setPosition(sf::Vector2f(context.window->getView().getSize().x - MAP_FONT_BOARDER, MAP_FONT_BOARDER)); //clw note
	topRightOrigin(mPlayerPosX);  // 在初始化的时候把长度定死，否则右对齐的时候，从999到1000会比较麻烦

	mPlayerPosY.setFont(font);
	mPlayerPosY.setString("yPlayer: 00000");
	mPlayerPosY.setCharacterSize(FONT_SIZE_MIDDLE);
	mPlayerPosY.setPosition(sf::Vector2f(context.window->getView().getSize().x - MAP_FONT_BOARDER, MAP_FONT_BOARDER) + sf::Vector2f(0.f, 50.f)); //clw note
	topRightOrigin(mPlayerPosY);
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
	if (mStatisticsUpdateTime >= sf::seconds(STATISTICS_UPDATE_TIME))
	{
		mStatisticsUpdateTime -= sf::seconds(STATISTICS_UPDATE_TIME);

		//FPS小数点后保留一位
		std::string strPosX = std::to_string(int(mPlayer.getPosition().x));
		int pos = strPosX.length();
		mPlayerPosX.setString("xPlayer: " + strPosX);
		//topRightOrigin(mPlayerPosX); // 因为strPosX长度一直在变化，所以每次更新都要重新让输出文字右上角对齐

		std::string strPosY = std::to_string(int(mPlayer.getPosition().y));
		pos = strPosY.length();
		mPlayerPosY.setString("yPlayer: " + strPosY);
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