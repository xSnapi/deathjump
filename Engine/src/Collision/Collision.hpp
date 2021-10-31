#pragma once
#include "Collider.hpp"

class Collision {
public:
	Collision(Collider& body);
	~Collision();

	bool CheckCollision(const Collision& other, sf::Vector2f& MTV) const;

	const Collider GetBody();
private:
	Collider& body;

	const static sf::Vector2f abs(sf::Vector2f v);
};