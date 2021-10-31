#pragma once
#include "stdafx.h"

class Camera {
public:
	static Camera& Get();

	const sf::View& GetView();

	void Update(const float& dt);

	void Shake(const float& duration);
private:
	Camera() { InitView(); }
	~Camera() {}

	sf::View m_view;

	sf::Vector2i m_positions[15];

	int m_currentPosition = 0;

	const float m_updateTime = 0.02f;
	float m_elapsedTime = 0.0f;
	
	float m_duration = 0.0f;

	static Camera s_instance;

	void ApplayEffect();
	void InitView();
};