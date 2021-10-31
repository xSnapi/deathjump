#include "stdafx.h"
#include "Seeker Bar.hpp"

SeekerBar::SeekerBar
(
	sf::Vector2f position,
	const sf::Font& font,
	const unsigned blocks,
	const unsigned currentBlock
)
	: m_blocks(blocks), m_currentBlock(currentBlock)
{
	for (unsigned i = 1; i <= m_blocks; i++) {
		const sf::Vector2f pos = sf::Vector2f(position.x + (m_size.x + 5) * i, position.y - m_size.y / 2.0f);

		AddQuad(pos);
	}

	InitButtons(position.y, font);
}

SeekerBar::~SeekerBar() {
	delete m_buttonLeft;
	delete m_buttonRight;
}

void SeekerBar::SetLeftFunction(std::function<void()> fun) {
	m_buttonLeft->SetFunction(fun);
}

void SeekerBar::SetRightFunction(std::function<void()> fun) {
	m_buttonRight->SetFunction(fun);
}

void SeekerBar::Update(const sf::Vector2f& mousePos) {
	m_buttonLeft->Update(mousePos);
	m_buttonRight->Update(mousePos);

	if (m_buttonLeft->Pressed()) {
		if (m_currentBlock > 0)
			m_currentBlock--;
	}
	else if (m_buttonRight->Pressed()) {
		if (m_currentBlock < m_blocks)
			m_currentBlock++;
	}

	for (int i = m_currentBlock; i < m_blocks; i++)
		SetQuadColor(i, sf::Color(180, 180, 180, 120));
	
	for (int i = 0; i < m_currentBlock; i++)
		SetQuadColor(i, sf::Color(220, 220, 220, 255));
}

void SeekerBar::SetCurrentBlock(const int block) {
	m_currentBlock = block;
}

void SeekerBar::AddQuad(const sf::Vector2f& pos) {
	std::array<sf::Vertex, 4> vertices;

	vertices[0].position = sf::Vector2f(pos.x,			  pos.y);
	vertices[1].position = sf::Vector2f(pos.x + m_size.x, pos.y);
	vertices[2].position = sf::Vector2f(pos.x + m_size.x, pos.y + m_size.y);
	vertices[3].position = sf::Vector2f(pos.x,			  pos.y + m_size.y);

	for (auto& v : vertices) {
		v.color = sf::Color(180, 180, 180, 255);
		m_vertices.push_back(v);
	}
}

void SeekerBar::SetQuadColor(size_t index, sf::Color color) {
	index *= 4;

	for (size_t i = 0; i < 4; i++)
		m_vertices[index + i].color = color;
}

void SeekerBar::InitButtons(const float& Y, const sf::Font& font) {

	const int offset = 50;

	{
		const sf::Vector2f pos = sf::Vector2f(m_vertices.front().position.x - offset, Y);
		m_buttonLeft = new Button("<", pos, font);
	}

	{
		const sf::Vector2f pos = sf::Vector2f(m_vertices[m_vertices.size() - 2].position.x + offset, Y);
		m_buttonRight = new Button(">", pos, font);
	}
}

void SeekerBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_vertices.data(), m_vertices.size(), sf::Quads);
	target.draw(*m_buttonLeft);
	target.draw(*m_buttonRight);
}
