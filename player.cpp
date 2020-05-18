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

Player::Player(sf::Texture& texture) // clw note: ע��������봫���ã����򴫹������ǿ���������ᱻ�ͷŵ��������Ļ����ʾ��Sprite����һ����ɫ�Ŀ��...
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
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second)) //�����ʵʱʱ�䣬�����Ҫ��mKeyBinding���У�			                                                               
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)  // ���������������� ����ʵʱ����
{
	// Traverse all assigned keys and check if they are pressed
	for(auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

// TODO����δʹ��
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

// TODO����δʹ��
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
	const float playerSpeed = PLAYER_SPEED; // clw note: 1s �� playerSpeed��pixel��

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
		rectangle.setFillColor(sf::Color(0, 0, 0, 0)); // clw note: set alpha = 0, ��ʵ�ľ��α��͸������
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

	////// 1����ɫ���߶�������
	// ע�����ƽ�ɫ�����ƶ��������ͼ�ı߽� ���ж���World���adaptPlayerPosition() 

	// �����л��������ߵ�framerow��Ҳ���ǵ�1��
	if (this->getVelocity().x < 0.f)
		mTextureRect.top = mTextureRect.height;
	// �����л��������ߵ�framerow��Ҳ���ǵ�2��
	if (this->getVelocity().x > 0.f)
		mTextureRect.top = 2 * mTextureRect.height;
	// �����л��������ߵ�framerow��Ҳ���ǵ�3��
	if (this->getVelocity().y < 0.f)
		mTextureRect.top = 3 * mTextureRect.height;
	// �����л��������ߵ�framerow��Ҳ���ǵ�0��
	if (this->getVelocity().y > 0.f)
		mTextureRect.top = 0;

	// ����ͣ��
	if (this->getVelocity().x == 0.f && this->getVelocity().y == 0.f)
	{
		mTextureRect.left = mTextureRect.width;
	}

	//���߶����������̸��£����Ǹ���ÿһ֡������ʱ����update
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
	
	////// 2���ƶ����൱��Entity��updateCurrent()����
	move(mVelocity * dt.asSeconds());
}