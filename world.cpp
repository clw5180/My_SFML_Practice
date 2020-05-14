#include "world.hpp"
#include "global.h"   // for WINDOW_HEIGHT and WINDOW_WIDTH

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>


World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldView.getSize().y / 2.f)
//, mScrollSpeed(-50.f)
, mScrollSpeed(0) 
, mPlayer(nullptr)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);

	mWorldBounds = sf::FloatRect(0, 0, float(mTextures.get(Textures::Background).getSize().x),
		float(mTextures.get(Textures::Background).getSize().y));
}

void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	// mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());	
	mPlayer->setVelocity(0.f, 0.f);

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);  //这里来根据获得的命令（行为）做更新，比如之前按了左键得到一个x-200的行为
	adaptPlayerVelocity();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt);
	adaptPlayerPosition();
}

void World::updateWorldView(sf::Time dt)
{
	////// 视角移动
	// （1）position x/y距边缘均大于窗口宽/高一半的区域，视角会正常推动；
	// （2）position y距上或下边缘小于等于窗口高一半，x距左右边缘均大于窗口宽一半，视角左右推动
	// （3）position x距左或右边缘小于等于窗口宽一半，y距上下边缘均大于窗口高一半，视角上下推动
	// （4）position x/y距边缘均小于窗口宽/高一半的区域，视角不推动
	sf::Vector2f playerPosition = mPlayer->getPosition();
	sf::Vector2f playerVelocity = mPlayer->getVelocity();


	// （1）
	if (playerPosition.x > WINDOW_WIDTH / 2 &&
		playerPosition.y > WINDOW_HEIGHT / 2 &&
		playerPosition.x < mWorldBounds.width - WINDOW_WIDTH / 2 &&
		playerPosition.y < mWorldBounds.height - WINDOW_HEIGHT / 2)
	{
		mWorldView.move(playerVelocity.x * dt.asSeconds(), playerVelocity.y * dt.asSeconds());
	}

	// （2）
	if (playerPosition.x > WINDOW_WIDTH / 2 &&
		playerPosition.x < mWorldBounds.width - WINDOW_WIDTH / 2 &&
		(playerPosition.y <= WINDOW_HEIGHT / 2 || 
		playerPosition.y >= mWorldBounds.height - WINDOW_HEIGHT / 2))
	{
		mWorldView.move(playerVelocity.x * dt.asSeconds(), 0);
	}

	// （3）
	if (playerPosition.y > WINDOW_HEIGHT / 2 &&
		playerPosition.y < mWorldBounds.height - WINDOW_HEIGHT / 2 &&
		(playerPosition.x <= WINDOW_WIDTH / 2 ||
		playerPosition.x >= mWorldBounds.width - WINDOW_WIDTH / 2))
	{
		mWorldView.move(0, playerVelocity.y * dt.asSeconds());
	}
}

World::~World()
{

}


void World::draw()
{
	mWindow.setView(mWorldView);  
	mWindow.draw(mSceneGraph);
	//	自注：draw的实现如下
	//  void RenderTarget::draw(const Drawable& drawable, const RenderStates& states)
	//	{
	//		drawable.draw(*this, states); // 这里this其实就是mWindow，其父类就是RenderTarget
	//	}

}


void World::loadTextures()
{
	mTextures.load(Textures::Player, "./Media/Textures/player.png");  // Eagle
	mTextures.load(Textures::Background, "./Media/Textures/background.jpg");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	sf::Texture& texture = mTextures.get(Textures::Background);

	sf::IntRect textureRect = sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
	//sf::IntRect textureRect(mWorldBounds);   
	//texture.setRepeated(true);  // clw note: the tiled background needed

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add player's aircraft
	std::unique_ptr<Player> leader(new Player(mTextures.get(Textures::Player))); 
	mPlayer = leader.get();
	mPlayer->setPosition(mSpawnPosition);
	mSceneLayers[Land]->attachChild(std::move(leader));
}

////// 角色行走，大地图角色边缘碰撞检测
void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = MAP_PLAYER_BOARDER;

	sf::Vector2f position = mPlayer->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayer->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayer->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayer->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	//mPlayer->accelerate(0.f, mScrollSpeed);
}
