#pragma once
#include "stdafx.h"
#include "Entity.hpp"

#include "Animations/Animator.hpp"
#include "Fixed Clock.hpp"

#include "../Effects/Light/Light.hpp"
#include "../Objects/Wall.hpp"

class Heart : public Entity {
public:
	Heart(sf::Texture* texture, const float& dt, const float& fdt);
	~Heart();

	virtual void FixedUpdate();
	virtual void Update();

	void ResolveCollision(std::vector<Wall>& walls);

	bool IsAlive() const;
	bool _IsAlive = true; // i should die for that
private:

	bool m_grounded = false;

	Light* m_light;

	Animator m_animation;

	FixedClock deSpawnClock;
	FixedClock realDeSpawnClock;
	FixedClock disappearClock;

	const float deSpawnTime = 3.0f;
	float disappearAnimationTime = 0.5f;

	bool disappear = false;

	unsigned currentColor = 0;

	const sf::Color colors[4] =
	{
		sf::Color(255, 255, 255, 80),
		sf::Color(255, 255, 255, 255),
		sf::Color(255, 255, 255, 80),
		sf::Color(255, 255, 255, 255),
	};

	void UpdateLight();
	void InitLight();

	void AnimateDisappear();

	void InitAnimations();
	void InitBody();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};