#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "menustate.hpp"
#include "utility.hpp"
#include "resourceholder.hpp"
#include "utility.hpp"

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mOptions()
, mOptionIndex(0)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Main);

	mBackgroundSprite.setTexture(texture);
	resizeBackgroundToWindow(mBackgroundSprite);
	
	// A simple menu demonstration
	sf::Text playOption;
	playOption.setFont(font);
	playOption.setString(L"新章初始");
	playOption.setCharacterSize(48u);
	//playOption.setLetterSpacing(2);
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f + sf::Vector2f(0.f, 100.f) );
	mOptions.push_back(playOption);

	sf::Text reloadOption;
	reloadOption.setFont(font);
	reloadOption.setString(L"前历再续");  // 如果要显示中文字符的话要用宽字符串(wstring)
	reloadOption.setCharacterSize(48u);
	//reloadOption.setLetterSpacing(2);
	centerOrigin(reloadOption);
	reloadOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 80.f));
	mOptions.push_back(reloadOption);

	sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString(L"返回太虚");
	exitOption.setCharacterSize(48u);
	//exitOption.setLetterSpacing(2);
	centerOrigin(exitOption);
	exitOption.setPosition(reloadOption.getPosition() + sf::Vector2f(0.f, 80.f));
	mOptions.push_back(exitOption);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);

	for(const sf::Text& text: mOptions)
		window.draw(text);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// The demonstration menu logic
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Return)
	{
		if (mOptionIndex == Play)
		{
			requestStackPop();
			requestStackPush(States::Game);
		}
		else if (mOptionIndex == Exit)
		{
			// The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
			requestStackPop();
		}
	}

	else if (event.key.code == sf::Keyboard::Up)
	{
		// Decrement and wrap-around
		if (mOptionIndex > 0)
			mOptionIndex--;
		else
			mOptionIndex = mOptions.size() - 1;

		updateOptionText();
	}

	else if (event.key.code == sf::Keyboard::Down)
	{
		// Increment and wrap-around
		if (mOptionIndex < mOptions.size() - 1)
			mOptionIndex++;
		else
			mOptionIndex = 0;

		updateOptionText();
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
		return;

	// White all texts
	for (sf::Text& text : mOptions)
	{ 
		//text.setColor(sf::Color::White);  //deprecated, Use setFillColor() or setOutlineColor() instead.
		//text.setFillColor(sf::Color::White);

		text.setFillColor(sf::Color(49, 23, 8));
		text.setOutlineColor(sf::Color(246, 229, 213));
		text.setOutlineThickness(3);
	}
	  

	// Red the selected text
	//mOptions[mOptionIndex].setColor(sf::Color::Red);
	mOptions[mOptionIndex].setFillColor(sf::Color::Red);
}
