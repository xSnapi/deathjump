#include "stdafx.h"
#include "Button.hpp"

#include "Key Check.hpp"
#include "../Effects/Music controller/Music Controller.hpp"

Button::Button
(
	std::string text, 
	sf::Vector2f position, 
	const sf::Font& font
) 
{
	m_label.setString(text);

	InitLabel(position, font);
	InitBody(position);
}

Button::~Button() {

}

void Button::Update(const sf::Vector2f& mousePos) {
	const sf::Color color = (m_body.getGlobalBounds().contains(mousePos) ? m_pressedColor : m_defaultColor);

	m_label.setFillColor(color);
	m_body.setOutlineColor(color);

	m_isPressed = color == m_pressedColor && sf::Mouse::isButtonPressed(sf::Mouse::Left) && KeyCheck::Button('L');

		
	if (m_isPressed && m_fun) {
		MusicController::Get().GetSound("button").play();
		m_fun();
	}
}

void Button::SetFunction(std::function<void()> fun) { m_fun = fun; }

const bool Button::Pressed() { return m_isPressed; }

const sf::Vector2f Button::GetSize() const {
	return m_body.getSize();
}

void Button::InitLabel(const sf::Vector2f& position, const sf::Font& font) {
	m_label.setFont(font);
	m_label.setCharacterSize(90);
	m_label.setSmooth(false);

	m_label.setPosition(position);
}

void Button::InitBody(const sf::Vector2f& position) {
	const sf::Vector2f size = m_label.getSize() + sf::Vector2f(30.0f, 20.0f);

	m_body.setFillColor(sf::Color(100, 100, 100, 80));

	m_body.setSize(size);
	m_body.setOrigin(size / 2.0f);
	m_body.setPosition(position);

	m_body.setOutlineThickness(4);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_body);
	target.draw(m_label);
}
