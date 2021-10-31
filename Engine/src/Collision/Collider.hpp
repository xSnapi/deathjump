#pragma once
#include <SFML/System/Vector2.hpp>

struct Collider 
{
	Collider
	(
		const sf::Vector2f& s = sf::Vector2f(),
		const sf::Vector2f& p = sf::Vector2f()
	)
		: size(s), position(p) { }

	sf::Vector2f size;
	sf::Vector2f position;
};