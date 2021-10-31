
#include "Collision.hpp"

Collision::Collision(Collider& body)
	: body(body)
{

}

Collision::~Collision() {

}

bool Collision::CheckCollision(const Collision& other, sf::Vector2f& MTV) const {
	const sf::Vector2f otherPosition = other.body.position;
	const sf::Vector2f otherSize = other.body.size / 2.0f;

	const sf::Vector2f thisPosition = this->body.position;
	const sf::Vector2f thisSize = this->body.size / 2.0f;

	const sf::Vector2f delta = otherPosition - thisPosition;
	const sf::Vector2f intersect = abs(delta) - (otherSize + thisSize);

	if (intersect.x < 0.0f && intersect.y < 0.0f) {
		if (intersect.x > intersect.y)
			MTV.x = (delta.x > 0.0f ? intersect.x : -intersect.x);
		else
			MTV.y = (delta.y > 0.0f ? intersect.y : -intersect.y);
		return true;
	}

	return false;
}

const sf::Vector2f Collision::abs(sf::Vector2f v) {
	v.x = (v.x < 0.0f ? -v.x : v.x);
	v.y = (v.y < 0.0f ? -v.y : v.y);

	return v;
}

const Collider Collision::GetBody() {
	return body;
}