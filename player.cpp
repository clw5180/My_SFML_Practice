#include "player.hpp"
#include "resourceholder.hpp"
#include "commandqueue.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

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

Player::Player(const TextureHolder& textures) 
: mSprite(textures.get(Textures::Player))
, mFrameSize(3, 4)  // hardcode
, mTimePerFrame(sf::seconds(0.25))
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
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
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

void Player::handleRealtimeInput(CommandQueue& commands)  // 处理上下左右这种实时输入
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
	const float playerSpeed = 200.f;

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
}

void Player::updateCurrent(sf::Time dt)
{
	updateAnimation(dt);

	// apply velocity
	Entity::updateCurrent(dt);
}

void Player::updateAnimation(sf::Time dt)
{
	int numFrames = mFrameSize.x * mFrameSize.y;

	mElapsedTime += dt;


	// TODO
	// 立刻切换到向左走的framerow，也就是第1行
	if (getVelocity().x < 0.f)
		mTextureRect.top = mTextureRect.height;
	// 立刻切换到向右走的framerow，也就是第2行
	if (getVelocity().x > 0.f)
		mTextureRect.top = 2 * mTextureRect.height;
	// 立刻切换到向上走的framerow，也就是第3行
	if (getVelocity().y < 0.f)
		mTextureRect.top = 3 * mTextureRect.height;
	// 立刻切换到向下走的framerow，也就是第0行
	if (getVelocity().y > 0.f)
		mTextureRect.top = 0;

	// 立刻停下
	if (getVelocity().x == 0.f && getVelocity().y == 0.f)
	{
		mTextureRect.left = mTextureRect.width;
	}

	//行走动画不会立刻更新，而是根据每一帧持续的时间来update
	while (mElapsedTime >= mTimePerFrame)
	{ 
		// go left
		if (getVelocity().x != 0.f or getVelocity().y != 0)
		{ 
			mTextureRect.left += mTextureRect.width;
			if (mTextureRect.left == 2 * mTextureRect.width)
				mTextureRect.left = 0;
		}

		// And progress to next frame
		mElapsedTime -= mTimePerFrame;
	}

	mSprite.setTextureRect(mTextureRect);
}