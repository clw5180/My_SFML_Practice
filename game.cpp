#include "game.h"
#include "state.hpp"
#include "stateIdentifiers.hpp"

#include "titlestate.hpp"
#include "menustate.hpp"
#include "gamestate.hpp"

#include "global.h"
#include "utility.hpp"   // for topLeftOrigin() 

#include <string>

Game* Game::s_pGame = new Game();
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); // FPS

Game::Game()
:mTextures()  
, mFonts()     
//, mPlayer()      // TODO������ȥ����ֻ��World�����
, mStateStack(State::Context(mWindow, mTextures, mFonts))
//, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Kalpa", sf::Style::Default); // TODO�����Կ���֧��sf::Style::Fullscreen����һ�������ļ��������Ƿ�ȫ�����ο�Cendric
	mWindow.setKeyRepeatEnabled(false); // you will only get a single event when the key is pressed.
	mWindow.setFramerateLimit(60);      // FPS����  TODO

	//����icon
	if (!mIcon.loadFromFile("./Media/icon.png"))
		throw std::runtime_error("ResourceHolder::load - Failed to load icon.png");
	mWindow.setIcon(mIcon.getSize().x, mIcon.getSize().y, mIcon.getPixelsPtr());

    // �������������  (clw note: ��Ϸ״̬���������ȣ�����World���������)
	mFonts.load(Fonts::Main, "./Media/Traditional_Ancient_India.ttf"); 
	mTextures.load(Textures::TitleScreen, "./Media/Textures/TitleScreen.jpg"); 
	mStatisticsText.setFont(mFonts.get(Fonts::Main));  
	
	mStatisticsText.setCharacterSize(FONT_SIZE_MIDDLE);
	//topRightOrigin(mStatisticsText);  //clw note: ��Ϊ��ʱmStatisticsTextΪ�գ�������һ�䲻�����κ����ã�getLocalBounds()�õ���ȫ����0
	mStatisticsText.setPosition(MAP_FONT_BOARDER, MAP_FONT_BOARDER);

	// ע�������״̬
	RegisterStates();

	//����һ��״̬��TitleState��push��״̬ջmStateStack��
	//mStateStack.pushState(States::Title);  
	mStateStack.pushState(States::Game);  // TODO��ֱ�ӽ�����Ϸ���������
}

Game::~Game()
{

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
		timeSinceLastUpdate += dt;  // dt means elapsedTime
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
	while (mWindow.pollEvent(event))  //Pop the event on top of the event queue����������waitEvent������
		                             //(1)һ�ο����ж���¼�; (2)���������䣬���ڲ��ᴦ�������û����κ��¼������ڡ�æµ��״̬
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

	// clw note: ֻ������������getDefaultView()����Text�Ż�����������֮����Ȼһֱ�ڴ������Ͻ� 
	//           ��Ϊ֮ǰ��World::draw()������� mWindow.setView(mWorldView)������Ϊ����ͼ����Ķ�����
	//           ������ɫ���Ǳ�SceneNodeͨ��attachChild()���ؽ����ģ����������ʵ�����������mSceneGraph,
	//           �Զ����»��ƵĹ����У�����һ�������SceneNode�ڵ���draw()��ʱ�򣬻�ͨ��
	//           states.transform *= getTransform() ��Ѹýڵ��Ƿ�translate��rotate��scale�ȱ任��Ϣ
	//           �����ӽڵ㣬����ӽڵ�����setPosition(80, -50)�Ȳ���ʱ����������ϲ�ڵ��λ�ö�����
	//           ����ڴ��ڵ�λ�ã���˱��������ʼ�ڴ������ĵ�(800, 600)�������ߵ�(900, 600)����ʱ���ӽ�
	//           mWorldView��ҪҲ��Ӧmove(100, 0)��Ȼ�����mWindow.setView(mWorldView)������Ķ���SceneNode
	//           ��Positionһֱ��(0, 0)
	//           ������ͨ������¶��㲢������SceneNode��sf::Text���󣬾���Ҫ����setView(mWindow.getDefaultView())��
	//           ����ʱ�����setPosition�Ȳ��������ǶԵ�ǰ ���� �����Ͻ����꣬Ҳ������������(100, 0)��������
	//           ���������ͼ������������(0, 0)
	//           ����View�����https://www.sfml-dev.org/tutorials/2.5/graphics-view.php
	mWindow.setView(mWindow.getDefaultView()); 


	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Game::UpdateStatistics(sf::Time dt)
{
	////// ����FPS  TODO
	
	////////////////////////////////////////////////////
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	//method 1
	//if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	//{
	//	mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

	//	mStatisticsUpdateTime -= sf::seconds(1.0f);
	//	mStatisticsNumFrames = 0;
	//}

	//method 2
	if (mStatisticsNumFrames == 60)
	{
		mStatisticsText.setString("FPS: " + std::to_string(1.0 / mStatisticsUpdateTime.asSeconds() * 60.0).substr(0, 4));

		//mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsUpdateTime = sf::Time::Zero;
		mStatisticsNumFrames = 0;
	}
	////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//method 3
	//mStatisticsUpdateTime += dt;
	//if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	//{
	//	mStatisticsText.setString("FPS: " + std::to_string(1000000.0f / dt.asMicroseconds()).substr(0, 4));
	//	mStatisticsUpdateTime -= sf::seconds(1.0f);
	//}
	/////////////////////////////////////////////////////////
}

void Game::RegisterStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	//mStateStack.registerState<PauseState>(States::Pause);
}