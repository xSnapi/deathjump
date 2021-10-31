#pragma once
#include "stdafx.h"

#include "Button.hpp"

class SeekerBar : public sf::Drawable {
public:
	SeekerBar() = default;
	SeekerBar(sf::Vector2f position, const sf::Font& font, const unsigned blocks, const unsigned currentBlock);
	~SeekerBar();

	void SetLeftFunction(std::function<void()> fun);
	void SetRightFunction(std::function<void()> fun);

	void Update(const sf::Vector2f& mousePos);

	void SetCurrentBlock(const int block);
private:
	unsigned m_blocks = 0;

	int m_currentBlock;

	sf::Vector2i m_size = sf::Vector2i(73, 73);
	std::vector<sf::Vertex> m_vertices;

	Button* m_buttonLeft  = nullptr;
	Button* m_buttonRight = nullptr;

	void SetQuadColor(size_t index, sf::Color color);
	void AddQuad(const sf::Vector2f& position);
	void InitButtons(const float& Y, const sf::Font& font);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};