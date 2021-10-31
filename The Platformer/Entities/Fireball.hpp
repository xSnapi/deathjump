#pragma once
#include "stdafx.h"
#include "Entity.hpp"

#include "Animations/Animator.hpp"
#include "../Effects/Particle Emitter/Particle Emitter.hpp"
#include "../Effects/Light/Light.hpp"

class Player;

class Fireball : public Entity {
public:
	Fireball(const float& dt, const float& fdt, sf::Texture* texture, sf::Texture* blowUp, float velocity, bool left);
	~Fireball();

	virtual void FixedUpdate();
	virtual void Update();

	bool Alive() const;
private:
	friend class Player; // please don't judge me for that im out of my mind

	sf::Sprite m_fireballSprite;
	sf::Sprite m_blowUpSprite;

	sf::Texture* m_blowTexture;

	std::unordered_map<std::string, Animator> m_Animations;

	sf::Vector2i m_size = sf::Vector2i(32, 32);
	
	ParticleEmitter* m_emitter;

	Light* m_light;

	bool m_left	  = true;
	bool m_alive  = true;
	bool m_blowUp = false;

	const float m_velocity = 150.0f;

	const float& m_dt;
	const float& m_fdt;

	const float m_positions[13] =
	{
		35.f,
		105.f,
		145.f,
		210.f,
		270.f,
		310.f,
		380.f,
		455.f,
		500.f,
		565.f,
		625.f,
		730.f,
		790.f,
	};

	void Move();

	void UpdateLight();
	void InitLight();

	void UpdateEmitter();
	void InitEmiiter();

	void Animate();
	void InitAnimations();
	void InitSprites();
	void InitCollider(sf::Vector2f position);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};