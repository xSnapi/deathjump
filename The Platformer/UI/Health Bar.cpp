#include "stdafx.h"
#include "Health Bar.hpp"

HealthBar::HealthBar() {
	InitBody();
}

HealthBar::~HealthBar() {

}

void HealthBar::Change(bool x) {

	if (x && m_currentCoords < 3)
		m_currentCoords++;
	else if (!x && m_currentCoords > 0)
		m_currentCoords--;

	//m_currentCoords += x ? 1 : -1;

	m_sprite.setTextureRect(sf::IntRect(0, m_coords[m_currentCoords], 96, 32));
}

int HealthBar::GetHealthState() const {
	return 3 - m_currentCoords;
}

void HealthBar::InitBody() {
	m_texture.loadFromFile("res/textures/health.png"); // just shut the fuck up we don't talk about it

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, m_coords[m_currentCoords], 96, 32));
	m_sprite.setScale(sf::Vector2f(2.0f, 2.0f));
	m_sprite.setPosition(sf::Vector2f(65.0f, 35.0f));
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_sprite);
}
