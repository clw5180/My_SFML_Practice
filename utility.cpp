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
	/* ���米��ͼƬΪ640x480��Ȼ�󴰿ڴ�С��1920x1080����Ҫ��һ��resize�Ĳ���
	/* ����transform�����ĵ�����Ϊsprite�����ģ�Ȼ���ƶ�sprite���������ģ��ٽ���resize����
	/************************************************************************/
	sf::FloatRect bounds = backgroundSprite.getLocalBounds();
	backgroundSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	backgroundSprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	backgroundSprite.setScale(sf::Vector2f(WINDOW_WIDTH / bounds.width, WINDOW_HEIGHT / bounds.height));
	/************************************************************************/
}
