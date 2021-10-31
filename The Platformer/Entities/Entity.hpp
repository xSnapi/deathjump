#pragma once
#include "stdafx.h"
#include "Collision/Collision.hpp"

class Entity : public sf::Drawable {
public:
	Entity(const float& dt, const float& fdt);
	virtual ~Entity();

	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;

	Collision GetCollider();
protected:
	Collider i_collider;

	sf::Drawable* i_drawable = nullptr;
	sf::Texture* i_texture	 = nullptr;
	sf::Shader* i_shader	 = nullptr;

	const float& i_fdt;
	const float& i_dt;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};