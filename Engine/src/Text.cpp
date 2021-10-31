#include "Text.hpp"

Text::Text() 
{

}

Text::~Text() 
{

}

const void Text::setFont(const sf::Font& font) {
	m_text::setFont(font);
	m_text::setOrigin(sf::Vector2f(unsigned(m_text::getGlobalBounds().width) / 2, unsigned(m_text::getGlobalBounds().height + m_text::getCharacterSize()) / 2));
}

const void Text::setFont(const sf::Font& font, unsigned size) {
	m_text::setFont(font);
	m_text::setCharacterSize(size);

	// i know it's hard coded
	const_cast<sf::Texture&>(font.getTexture(size)).setSmooth(false);

	m_text::setOrigin(sf::Vector2f(unsigned(m_text::getGlobalBounds().width) / 2, unsigned(m_text::getGlobalBounds().height + m_text::getCharacterSize()) / 2));
}

const void Text::setCharacterSize(unsigned size) {
	m_text::setCharacterSize(size);
	m_text::setOrigin(sf::Vector2f(unsigned(m_text::getGlobalBounds().width) / 2, unsigned(m_text::getGlobalBounds().height + size) / 2));
}

const void Text::reposition() {
	sf::Vector2f pos = m_text::getPosition();
	m_text::setOrigin(sf::Vector2f(unsigned(m_text::getGlobalBounds().width) / 2, unsigned(m_text::getGlobalBounds().height + m_text::getCharacterSize()) / 2));

	m_text::setPosition(pos);
}

const void Text::setSmooth(bool x) {
	sf::Font* font = const_cast<sf::Font*>(m_text::getFont());

	if (!font)
		return;

	const_cast<sf::Texture&>(font->getTexture(m_text::getCharacterSize())).setSmooth(x);
}

const sf::Vector2f Text::getSize() {
	return sf::Vector2f(m_text::getGlobalBounds().width, m_text::getGlobalBounds().height);
}
