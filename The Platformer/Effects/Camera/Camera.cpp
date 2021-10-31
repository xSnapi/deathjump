#include "stdafx.h"
#include "Camera.hpp"

#include "Random.hpp"

Camera& Camera::Get() {
	return s_instance;
}

const sf::View& Camera::GetView() {
	return m_view;
}

void Camera::Update(const float& dt) {
	m_elapsedTime += dt;
	while (m_elapsedTime > m_updateTime) {
		ApplayEffect();
		m_elapsedTime -= m_updateTime;
	}
}

void Camera::Shake(const float& duration) {
	m_duration = duration;

	m_currentPosition = 0;

	for (int i = 0; i < 15; i++)
		m_positions[i] = sf::Vector2i(Random::RandInt(-12, 12), Random::RandInt(-12, 12));
}

void Camera::ApplayEffect() {
	if (m_duration > 0.0f) {
		m_duration -= 0.1f;

		m_view.setCenter(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT) / 2.0f + (sf::Vector2f)m_positions[m_currentPosition]);

		m_currentPosition++;
		if (m_currentPosition > 9)
			m_currentPosition = 0;

		if (m_duration <= 0.0f)
			m_duration = 0.0f;
	}

	if (m_duration == 0.0f) {
		m_view.setCenter(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT) / 2.0f);
	}
}

void Camera::InitView() {
	m_view.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	m_view.setCenter(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT) / 2.0f);
}

Camera Camera::s_instance;