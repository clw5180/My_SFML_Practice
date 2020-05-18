#include "player.hpp"
#include "resourceholder.hpp"
#include "commandqueue.hpp"
#include "world.hpp"
#include "global.h"     // for PLAYER_SPEED

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp> // for debug

#include <map>
#include <string>
#include <algorithm>


struct PlayerMover
{
	PlayerMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

	void operator() (Player& player, sf::Time) const
	{
		player.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

//Player::Player()
//{
//	// Set initial key bindings
//	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
//	mKeyBinding[sf::Keyboard::Right] = MoveRight;
//	mKeyBinding[sf::Keyboard::Up] = MoveUp;
//	mKeyBinding[sf::Keyboard::Down] = MoveDown;
//
//	// Set initial action bindings
//	initializeActions();
//
//	// Assign all categories to player's aircraft
//	for (auto& pair : mActionBinding)
//		pair.second.category = Category::PlayerAircraft; //TODO
//}

Player::Player(sf::Texture& texture) // clw note: 注意这里必须传引用，否则传过来的是拷贝，纹理会被释放掉，造成屏幕上显示的Sprite就是一个白色的框框...
: mSprite(texture)
, mFrameSize(3, 4)  // hardcode
, mFrameDuration(sf::seconds(FRAME_DURATION))
{
	mTextureRect = sf::IntRect(mSprite.getTextureRect().width / mFrameSize.x, 0, mSprite.getTextureRect().width / mFrameSize.x, mSprite.getTextureRect().height / mFrameSize.y);  //clw note: TODO 

	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::Down] = MoveDown;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's aircraft
	for (auto& pair : mActionBinding)
		pair.second.category = Category::Player; //TODO

	// Set the Sprite
	//sf::FloatRect bounds = mSprite.getLocalBounds();
	//mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	mSprite.setOrigin(mTextureRect.width / 2.f, mTextureRect.height / 2.f);
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second)) //处理非实时时间，这个键要在mKeyBinding里有；			                                                               
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)  // 处理上下左右行走 这种实时输入
{
	// Traverse all assigned keys and check if they are pressed
	for(auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

// TODO：暂未使用
void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

// TODO：暂未使用
sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for(auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	const float playerSpeed = PLAYER_SPEED; // clw note: 1s 走 playerSpeed个pixel，

	mActionBinding[MoveLeft].action = derivedAction<Player>(PlayerMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Player>(PlayerMover(+playerSpeed, 0.f));
	mActionBinding[MoveUp].action = derivedAction<Player>(PlayerMover(0.f, -playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Player>(PlayerMover(0.f, +playerSpeed));
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
			return true;

		default:
			return false;
	}
}

unsigned int Player::getCategory() const
{
	return Category::Player;
}

void Player::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);

	if (DEBUG)
	{
		sf::FloatRect boundingBox = mSprite.getGlobalBounds();
		sf::RectangleShape rectangle(sf::Vector2f(boundingBox.width, boundingBox.height));
		rectangle.setPosition(boundingBox.left, boundingBox.top);
		rectangle.setOutlineColor(sf::Color(255, 0, 0));
		rectangle.setOutlineThickness(2.0);
		rectangle.setFillColor(sf::Color(0, 0, 0, 0)); // clw note: set alpha = 0, 从实心矩形变成透明矩形
		target.draw(rectangle, states);
	}
}

void Player::updateCurrent(sf::Time dt)
{
	updateAnimation(dt);
}

//TODO 
void Player::updateAnimation(sf::Time dt)
{
	int numFrames = mFrameSize.x * mFrameSize.y;

	mElapsedTime += dt;

	////// 1、角色行走动画更新
	// 注意限制角色不能移动超出大地图的边界 的判断在World类的adaptPlayerPosition() 

	// 立刻切换到向左走的framerow，也就是第1行
	if (this->getVelocity().x < 0.f)
		mTextureRect.top = mTextureRect.height;
	// 立刻切换到向右走的framerow，也就是第2行
	if (this->getVelocity().x > 0.f)
		mTextureRect.top = 2 * mTextureRect.height;
	// 立刻切换到向上走的framerow，也就是第3行
	if (this->getVelocity().y < 0.f)
		mTextureRect.top = 3 * mTextureRect.height;
	// 立刻切换到向下走的framerow，也就是第0行
	if (this->getVelocity().y > 0.f)
		mTextureRect.top = 0;

	// 立刻停下
	if (this->getVelocity().x == 0.f && this->getVelocity().y == 0.f)
	{
		mTextureRect.left = mTextureRect.width;
	}

	//行走动画不会立刻更新，而是根据每一帧持续的时间来update
	while (mElapsedTime >= mFrameDuration)
	{ 
		// go left
		if (this->getVelocity().x != 0.f or this->getVelocity().y != 0)
		{ 
			mTextureRect.left += mTextureRect.width;
			if (mTextureRect.left == 2 * mTextureRect.width)
				mTextureRect.left = 0;
		}

		// And progress to next frame
		mElapsedTime -= mFrameDuration;
	}
	mSprite.setTextureRect(mTextureRect);
	
	////// 2、移动，相当于Entity的updateCurrent()方法
	move(mVelocity * dt.asSeconds());
}