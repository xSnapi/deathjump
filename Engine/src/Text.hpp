#pragma once
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Text.hpp"

#include <string>

class Text : public sf::Text {
public:
	Text();
	~Text();

	const void setFont(const sf::Font& font);
	const void setFont(const sf::Font& font, unsigned size);
	const void setCharacterSize(unsigned size);

	const void reposition();

	const void setSmooth(bool x);

	const sf::Vector2f getSize();

public:
	using sf::Text::getPosition;
	using sf::Text::setPosition;
	using sf::Text::move;

	using sf::Text::setLetterSpacing;

	using sf::Text::setOutlineThickness;
	using sf::Text::setOutlineColor;

	using sf::Text::setFillColor;

	using sf::Text::setString;

private:
	typedef sf::Text m_text;
};