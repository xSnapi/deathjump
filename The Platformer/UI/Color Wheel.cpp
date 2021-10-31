#include "stdafx.h"
#include "Color Wheel.hpp"

	// do not fucking look in here i will kill you
ColorWheel::ColorWheel(sf::Vector2f position) {
	
	sf::Image image;

    image.create(256, 256);

	for (int y = 0; y < 200; y++) {
		for (int x = 255; x >= 0; x--) {
			sf::Vector3f color;

			float sub = 0.0f;

			if (x < 32) {
				color = Lerp(sf::Vector3f(255, 0, 0), sf::Vector3f(255, 0, 0), ((float)x - sub) / 32.0f);
			}
			else if (x >= 32 && x < 64) {
				sub = 32.0;
				color = Lerp(sf::Vector3f(255, 0, 0), sf::Vector3f(255, 255, 0), ((float)x - sub) / 32.0f);
			}
			else if (x >= 64 && x < 96) {
				sub = 64.0;
				color = Lerp(sf::Vector3f(255, 255, 0), sf::Vector3f(0, 255, 0), ((float)x - sub) / 32.0f);
			}
			else if (x >= 96 && x < 128) {
				sub = 96.0;
				color = Lerp(sf::Vector3f(0, 255, 0), sf::Vector3f(0, 255, 255), ((float)x - sub) / 32.0f);
			}
			else if (x >= 128 && x < 160) {
				sub = 128.0f;
				color = Lerp(sf::Vector3f(0, 255, 255), sf::Vector3f(0, 0, 255), ((float)x - sub) / 32.0f);
			}
			else if (x >= 160 && x < 192) {
				sub = 160.0f;
				color = Lerp(sf::Vector3f(0, 0, 255), sf::Vector3f(255, 0, 255), ((float)x - sub) / 32.0f);
			}
			else if (x >= 192 && x < 224) {
				sub = 192.0f;
				color = Lerp(sf::Vector3f(255, 0, 255), sf::Vector3f(255, 255, 255), ((float)x - sub) / 32.0f);
			}
			else if (x >= 224) 
				color = sf::Vector3f(255.0f, 255.0f, 255.0f);

			image.setPixel(x, y, sf::Color(color.x, color.y, color.z, 255));
		}

	}

	
	for (int x = 255; x >= 0; x--) {
		sf::Color ccolor = image.getPixel(x, 199);
		sf::Vector3f color = sf::Vector3f(ccolor.r, ccolor.g, ccolor.b);

		for (int y = 200; y < 256; y++) {
			color = Lerp(color, sf::Vector3f(0.0f, 0.0f, 0.0f), ((float)y - 200) / 384.0f);
			image.setPixel(x, y, sf::Color(color.x, color.y, color.z, 255));
		}
	}

    m_texture.loadFromImage(image);
	m_body.setTextureRect(sf::IntRect(0, 0, 256, 256));
    m_body.setTexture(&m_texture);
	m_body.setSize(sf::Vector2f(256, 256));
	m_body.setOutlineThickness(3);
	m_body.setOrigin(sf::Vector2f(256, 256) / 2.0f);
	m_body.setPosition(position);
}

ColorWheel::~ColorWheel() {

}

sf::Vector3f ColorWheel::GetPixel(sf::Vector2f pos) {
	pos -= m_body.getPosition() - m_body.getSize() / 2.0f;

	if (pos.x < 0 || pos.x > 255)
		return sf::Vector3f();

	if (pos.y < 0 || pos.y > 255)
		return sf::Vector3f();

	sf::Color color = m_texture.copyToImage().getPixel(pos.x, pos.y);

	return sf::Vector3f(color.r, color.g, color.b);
}

sf::FloatRect ColorWheel::getGlobalBounds() {
	return m_body.getGlobalBounds();
}


sf::Vector3f ColorWheel::Lerp(const sf::Vector3f& x, const sf::Vector3f& y, const float& t) {
	return x * (1.f - t) + y * t;
}

void ColorWheel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_body);
}
