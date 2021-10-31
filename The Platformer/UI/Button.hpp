#pragma once
#include "stdafx.h"
#include "Text.hpp"

class Button : public sf::Drawable {
public:
	Button() = default;
	Button(std::string text, sf::Vector2f position, const sf::Font& font);
	~Button();

	void Update(const sf::Vector2f& mousePos);

	void SetFunction(std::function<void()> fun);

	// Accessors
	const bool Pressed();
	const sf::Vector2f GetSize() const;

private:
	Text m_label;
	sf::RectangleShape m_body;

	bool m_isPressed = false;

	std::function<void()> m_fun = 0;

	const sf::Color m_pressedColor = sf::Color(100, 100, 100, 255);
	const sf::Color m_defaultColor = sf::Color(255, 255, 255, 255);

	void InitLabel(const sf::Vector2f& position, const sf::Font& font);
	void InitBody(const sf::Vector2f& position);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};