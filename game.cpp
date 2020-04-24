#include <string>


#include "game.h"
#include "state.hpp"
#include "stateIdentifiers.hpp"

#include "titlestate.hpp"
#include "menustate.hpp"
#include "gamestate.hpp"

#include "global.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
:mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Menus", sf::Style::Default)
, mTextures()  
, mFonts()     
//, mPlayer()      // TODO������ȥ����ֻ��World�����
, mStateStack(State::Context(mWindow, mTextures, mFonts))
//, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false); // you will only get a single event when the key is pressed.
	mWindow.setFramerateLimit(60);      // TODO

	//����icon
	if (!mIcon.loadFromFile("./Media/icon.png"))
		throw std::runtime_error("ResourceHolder::load - Failed to load icon.png");
	mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());

    // �������������  (clw note: ��Ϸ״̬���������ȣ�����World���������)
	mFonts.load(Fonts::Main, "./Media/Traditional_Ancient_India.ttf"); 
	mTextures.load(Textures::TitleScreen, "./Media/Textures/TitleScreen.jpg"); 
	mStatisticsText.setFont(mFonts.get(Fonts::Main));  
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(36u);

	// ע�������״̬
	RegisterStates();

	//����һ��״̬��TitleState��push��״̬ջmStateStack��
	mStateStack.pushState(States::Title);

}

void Game::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	/************************************************************************/
	/* 3����Сʱ�䲽��������˶�̬ʱ�䲽���͹̶�ʱ�䲽���ķ�����
	/* ͨ��ȷ������ update(dt) ������ʱ���������ô�ߣ��̶�ΪTimePerFrame��������Ⱦ����ʵʱ���У�600FPSҲok��
	/* Ҳ��������ͨ�����������������С��֡��������û�����ġ�
	/* ���https://blog.csdn.net/qq_33567644/article/details/91126517?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1
	/************************************************************************/
	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart(); //puts the time counter back to zero; returns the time elapsed
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			ProcessInput();
			Update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		UpdateStatistics(dt);
		Render();
	}
}

void Game::ProcessInput()
{
	sf::Event event;  // ��Ҫ #include <SFML/Window/Event.hpp>����state.hpp��
	while (mWindow.pollEvent(event))  // ���򴰿ڻ�һֱ��æµ״̬
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Game::Update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Game::Render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Game::UpdateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::RegisterStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	//mStateStack.registerState<PauseState>(States::Pause);
}