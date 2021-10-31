#pragma once
#include "stdafx.h"
#include "Collision/Collision.hpp"

//TODO: REMOVE DRAWABLE
class Wall{
public:
	Wall(sf::Vector2f position, sf::Vector2f size);
	~Wall();

	Collision GetCollider();

private:
	Collider m_collider;

	void InitCollider(const sf::Vector2f& position, const sf::Vector2f& size);
};