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
//, mPlayer()      // TODO：这里去掉，只在World里面加
, mStateStack(State::Context(mWindow, mTextures, mFonts))
//, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false); // you will only get a single event when the key is pressed.
	mWindow.setFramerateLimit(60);      // TODO

	//载入icon
	if (!mIcon.loadFromFile("./Media/icon.png"))
		throw std::runtime_error("ResourceHolder::load - Failed to load icon.png");
	mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());

    // 载入字体和纹理  (clw note: 游戏状态里面的纹理等，是在World类里面加载)
	mFonts.load(Fonts::Main, "./Media/Traditional_Ancient_India.ttf"); 
	mTextures.load(Textures::TitleScreen, "./Media/Textures/TitleScreen.jpg"); 
	mStatisticsText.setFont(mFonts.get(Fonts::Main));  
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(36u);

	// 注册所需的状态
	RegisterStates();

	//将第一个状态（TitleState）push到状态栈mStateStack中
	mStateStack.pushState(States::Title);

}

void Game::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	/************************************************************************/
	/* 3、最小时间步长：结合了动态时间步长和固定时间步长的方法。
	/* 通过确保传入 update(dt) 方法的时间参数不那么高（固定为TimePerFrame），而渲染可以实时进行（600FPS也ok）
	/* 也就是我们通过这个方法设置了最小的帧数，但是没有最大的。
	/* 详见https://blog.csdn.net/qq_33567644/article/details/91126517?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1
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
	sf::Event event;  // 需要 #include <SFML/Window/Event.hpp>，在state.hpp有
	while (mWindow.pollEvent(event))  // 否则窗口会一直在忙碌状态
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