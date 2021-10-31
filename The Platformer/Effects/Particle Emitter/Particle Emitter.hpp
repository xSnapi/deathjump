#pragma once
#include "stdafx.h"

// it's not a good patricle class but it's good enough for me

struct Particle
{
	Particle(float lifeTime, sf::Vector2f direction) 
		: lifeTime(lifeTime), constLifeTime(lifeTime), direction(direction)
	{
	
	}

	float lifeTime		= 0.0f;
	float constLifeTime = 0.0f;
	sf::Vector2f direction;
};

class ParticleEmitter : public sf::Drawable {
public:
	ParticleEmitter(const sf::Color birthColor, const sf::Color deathColor, unsigned size);
	~ParticleEmitter();

	void Update(const float& dt);
	void AddParticles();

	void SetPosition(const sf::Vector2f& pos);
	void SetDirection(const sf::Vector2f& dir);

	void SetLifeTimeRange(const float& min, const float& max);

	void SetSpeed(const float& speed);

	void SetBirthColor(sf::Color color);
	void SetDeathColor(sf::Color color);

	void SetParticleMax(unsigned max);
	void SetSpread(float spred);

	void SetSize(unsigned size);

	void Clear();
private:
	sf::Vector3f m_birthColor;
	sf::Vector3f m_deathColor;

	const float m_updateTime	= 0.015f;
	float m_movementSpeed		= 5.5f;
	float m_elapsedTime			= 0.0f;

	unsigned m_size = 0;
	unsigned m_max = 50;

	float m_lifeTimeMin = 0.0f;
	float m_lifeTimeMax = 0.0f;

	float m_spread = 0.3f;

	sf::Vector2f m_position;
	sf::Vector2f m_direction;

	std::vector<sf::Vertex> m_vertices;
	std::vector<Particle> m_particles;

	void UpdateParticles();

	void AddQuad(const sf::Vector2f pos);
	void DeleteQuad(unsigned index);
	void SetQuadColor(unsigned index, const sf::Color& color);
	void SetQuadScale(unsigned index, const float& scale);
	void SetQuadRotation(unsigned index, const float& angle);
	void MoveQuad(unsigned index, const sf::Vector2f& dir);

	sf::Vector3f Lerp(const sf::Vector3f& x, const sf::Vector3f& y, const float& t);
	float Lerp(const float& x, const float& y, const float& t);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};