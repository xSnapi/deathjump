#include "stdafx.h"
#include "Wall.hpp"

Wall::Wall(sf::Vector2f position, sf::Vector2f size) {
	InitCollider(position, size);
}

Wall::~Wall() {

}

Collision Wall::GetCollider() {
	return m_collider;
}

void Wall::InitCollider(const sf::Vector2f& position, const sf::Vector2f& size) {
	m_collider.size		= size;
	m_collider.position = position;
}
