#include "stdafx.h"
#include "Light.hpp"

#include "Random.hpp"

Light::Light
(
	sf::Vector2f position,
	float radius
)
	: m_shader(ShaderLibrary::Get().GetShader("light")), m_position(position), m_radius(radius)
{
	m_body.setSize(sf::Vector2f(radius, radius));
	m_body.setOrigin(sf::Vector2f(radius, radius) / 2.0f);
	m_body.setTextureRect(sf::IntRect(0, 0, 1, 1));
	m_body.setPosition(position);

	const int r1 = Random::RandInt(1, 10);
	const int r2 = Random::RandInt(5, 20);

	m_move = (float)r1 / (float)r2;
}

Light::~Light() {

}

void Light::SetPosition(sf::Vector2f position) {
	m_position = position;
	m_body.setPosition(position);
}

void Light::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	m_shader.setUniform("u_move", m_move);
	target.draw(m_body, &m_shader);
}
