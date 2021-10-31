#include "stdafx.h"
#include "Color Picker.hpp"

#include "Key Check.hpp"

ColorPicker::ColorPicker
(
	sf::Color& color, 
	sf::Vector2f position
) 
	: m_color(&color)
{
	InitBody(position);
	m_colorWheel = new ColorWheel(position);
}

ColorPicker::~ColorPicker() {
	delete m_colorWheel;
}

void ColorPicker::Update(const sf::Vector2f& mousePos) {
	if (m_wheelVisible) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_keyCheck) {
			m_wheelVisible = false;

			if (m_colorWheel->getGlobalBounds().contains(mousePos)) {
				sf::Vector3f color = m_colorWheel->GetPixel(mousePos);
				*m_color = sf::Color(color.x, color.y, color.z, 255);
				m_body.setFillColor(*m_color);
			}
		}
	}
	else {
		bool pressed = m_body.getGlobalBounds().contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_keyCheck;

		if (pressed) {
			m_wheelVisible = true;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_keyCheck = true;
	else
		m_keyCheck = false;
}

void ColorPicker::InitBody(const sf::Vector2f& position) {
	const sf::Vector2f size = sf::Vector2f(75.0f, 75.0f);

	m_body.setSize(size);
	m_body.setOrigin(size / 2.0f);
	m_body.setPosition(position);
	m_body.setFillColor(*m_color);
	m_body.setOutlineThickness(3);
}

void ColorPicker::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (m_wheelVisible)
		target.draw(*m_colorWheel);
	else
		target.draw(m_body);

	// for some reson i couldn't write ? expression here B))
}
