#pragma once
#include "stdafx.h"
#include "Shader Library.hpp"

class Light : public sf::Drawable {
public:
	Light(sf::Vector2f position, float radius);
	~Light();

	void SetPosition(sf::Vector2f position);
private:
	float m_radius = 0.0f;
	sf::Vector2f m_position;

	sf::Shader& m_shader;
	sf::RectangleShape m_body;

	float m_move = 0.0f;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};