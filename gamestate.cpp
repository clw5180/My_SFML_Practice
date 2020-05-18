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
	mPlayerPosX.setString("xPlayer: 00000"); // clw TODO: ռλ���������999��1000��ʱ����������ϽǶ�����ᶶ��
	mPlayerPosX.setCharacterSize(FONT_SIZE_MIDDLE);
	//playOption.setLetterSpacing(2);
	mPlayerPosX.setPosition(sf::Vector2f(context.window->getView().getSize().x - MAP_FONT_BOARDER, MAP_FONT_BOARDER)); //clw note
	topRightOrigin(mPlayerPosX);  // �ڳ�ʼ����ʱ��ѳ��ȶ����������Ҷ����ʱ�򣬴�999��1000��Ƚ��鷳

	mPlayerPosY.setFont(font);
	mPlayerPosY.setString("yPlayer: 00000");
	mPlayerPosY.setCharacterSize(FONT_SIZE_MIDDLE);
	mPlayerPosY.setPosition(sf::Vector2f(context.window->getView().getSize().x - MAP_FONT_BOARDER, MAP_FONT_BOARDER) + sf::Vector2f(0.f, 50.f)); //clw note
	topRightOrigin(mPlayerPosY);
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
	if (mStatisticsUpdateTime >= sf::seconds(STATISTICS_UPDATE_TIME))
	{
		mStatisticsUpdateTime -= sf::seconds(STATISTICS_UPDATE_TIME);

		//FPSС�������һλ
		std::string strPosX = std::to_string(int(mPlayer.getPosition().x));
		int pos = strPosX.length();
		mPlayerPosX.setString("xPlayer: " + strPosX);
		//topRightOrigin(mPlayerPosX); // ��ΪstrPosX����һֱ�ڱ仯������ÿ�θ��¶�Ҫ����������������ϽǶ���

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