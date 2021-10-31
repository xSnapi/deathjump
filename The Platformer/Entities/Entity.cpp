#include "stdafx.h"
#include "Entity.hpp"

Entity::Entity
(
	const float& dt, 
	const float& fdt
) 
	: i_dt(dt), i_fdt(fdt)
{

}

Entity::~Entity() {
	delete i_drawable;
}

Collision Entity::GetCollider() { return i_collider; }

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(*i_drawable, i_shader); }
//void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const { 
//	sf::RectangleShape shape;
//	shape.setSize(i_collider.size);
//	shape.setOrigin(i_collider.size / 2.0f);
//	shape.setPosition(i_collider.position);
//
//	target.draw(shape);
//}
