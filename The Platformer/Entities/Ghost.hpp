#pragma once
#include "stdafx.h"

#include "Entity.hpp"

#include "Animations/Animator.hpp"
#include "../Effects/Light/Light.hpp"
#include "../Effects/Particle Emitter/Particle Emitter.hpp"

class Ghost : public Entity {
public:
	Ghost(sf::Texture* texture, const float& dt, const float& fdt);
	~Ghost();

	virtual void FixedUpdate();
	virtual void Update() {};
	void Update(Collider obj);

	// i should die for that too ;)
	bool m_isAlive = true;
	bool m_blowUp = false;
	Animator m_animator;

	ParticleEmitter* m_emitter;
	bool m_left = false;
private:
	sf::Sprite* m_sprite;

	Light* m_light;

	sf::Clock m_clock;

	sf::Vector2f m_dir;

	float m_lifeTime = 12.0f;

	float m_lightVal = 0.55f;

	float Lerp(const float& x, const float& y, const float& t);
	
	void InitEmitter();

	void UpdateLight();
	void InitLight();

	void InitAnimator();
	void InitBody();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};