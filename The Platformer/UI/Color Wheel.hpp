#pragma once
#include "stdafx.h"

// yeah we don't need to know what is inside here c:
class ColorWheel : public sf::Drawable {
public:
	ColorWheel(sf::Vector2f position);
	~ColorWheel();

	sf::Vector3f GetPixel(sf::Vector2f pos);

	sf::FloatRect getGlobalBounds();

private:
	sf::Texture m_texture;
	sf::RectangleShape m_body;

	sf::Vector3f Lerp(const sf::Vector3f& x, const sf::Vector3f& y, const float& t);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};