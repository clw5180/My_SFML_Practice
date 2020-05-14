#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cmath>

#include "utility.hpp"
#include "global.h"

void centerOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void topLeftOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left), std::floor(bounds.top));
}

void topRightOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width), std::floor(bounds.top));
}

void resizeBackgroundToWindow(sf::Sprite& backgroundSprite)
{
	/************************************************************************/
	/* 比如背景图片为640x480，然后窗口大小是1920x1080，需要做一个resize的操作
	/* 首先transform的中心点设置为sprite的中心，然后移动sprite到窗口中心，再进行resize即可
	/************************************************************************/
	sf::FloatRect bounds = backgroundSprite.getLocalBounds();
	backgroundSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	backgroundSprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	backgroundSprite.setScale(sf::Vector2f(WINDOW_WIDTH / bounds.width, WINDOW_HEIGHT / bounds.height));
	/************************************************************************/
}
