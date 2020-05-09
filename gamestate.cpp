#include "gamestate.hpp"
#include "utility.hpp"  // for centerOrigin()
#include "global.h"     // for FONT_SIZE_LARGE

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

// TODO��mWorld��GameState�У�mPlayer��mWorld��
GameState::GameState(StateStack& stack, Context context)  
: State(stack, context)
, mWorld(*context.window)
, mPlayer(*mWorld.getPlayer())  //TODO
, mStatisticsUpdateTime()
{
	sf::Font& font = context.fonts->get(Fonts::Main);

	mPlayerPosX.setFont(font);
	mPlayerPosX.setString("X_PlayerPos: 0000"); // clw TODO: ռλ������������1000.0Ҳ����ʾ����
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

	//һЩ���ԵĲ�����ӡ����Ļ�����ɫ�����
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mPlayerPosX);
	window.draw(mPlayerPosY);
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);  // ���³���ͼSceneGraph 
	mWorld.updateWorldView(dt); //���ݽ�ɫλ�ø����ӽ�
	sf::View windowView = this->getContext().window->getView();

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);  // clw modify

	////// ���³�������
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