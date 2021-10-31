#pragma once
#include "stdafx.h"
#include "Entity.hpp"

#include "Timer.hpp"

#include "Animations/Animator.hpp"

#include "../Objects/Wall.hpp"
#include "Heart.hpp"
#include "Ghost.hpp"
#include "../Effects/Particle Emitter/Particle Emitter.hpp"

#include "../UI/Health Bar.hpp"

class Player : public Entity {
public:
	Player(const float& dt, const float& fdt, sf::Texture* texture);
	~Player();

	virtual void FixedUpdate();
	virtual void Update();

	void ResolveCollision(Entity* entity);
	void ResolveCollision(Ghost* entity, std::vector<Ghost*>& ghosts);
	void ResolveCollision(Heart* entity);
	void ResolveCollision(std::vector<Wall>& walls);

	bool IsAlive() const;
private:
	sf::Vector2f m_velocity;

	bool m_alive = true;

	const float m_movementSpeed = 420.0f;
	float m_torque				= 0.85f;
	const float m_gravity		= 4605.0f;
	const float m_jumpVelocity  = -1200.0f;

	bool m_grounded		= true;
	bool m_crouching	= false;
	bool m_grabbing		= false;

	bool m_hit			= true;
	bool m_movable		= true;

	bool m_wasGrounded  = false;

	unsigned m_currentHitTake = 10;

	const sf::Vector2i m_playerSize = sf::Vector2i(96, 84);
	std::unordered_map<std::string, Animator> m_Animations;

	ParticleEmitter* m_emitter;
	ParticleEmitter* m_fallEmitter;

	sf::Clock m_hitTimer;

	// amogus
	HealthBar m_healthBar;

	sf::SoundBuffer m_ghostBuffer;
	sf::Sound m_ghost;

	//Timer m_hitTimer; we don't talk about it

	void UpdateTimer();

	void Animate();

	void KeepInBorders();

	void Crouch();

	void Move();
	void UpdateVelocity();

	void UpdateEmitters();
	void InitEmitters();

	void InitAnimations();
	void InitBody();
	void InitCollider();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};