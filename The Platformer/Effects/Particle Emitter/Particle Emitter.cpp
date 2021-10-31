#include "stdafx.h"
#include "Particle Emitter.hpp"

#include "Random.hpp"

ParticleEmitter::ParticleEmitter
(
	const sf::Color birthColor, 
	const sf::Color deathColor, 
	unsigned size
) 
	: m_size(size)
{
	m_birthColor = sf::Vector3f(birthColor.r, birthColor.g, birthColor.b);
	m_deathColor = sf::Vector3f(deathColor.r, deathColor.g, deathColor.b);
}

ParticleEmitter::~ParticleEmitter() {

}

void ParticleEmitter::Update(const float& dt) {
	m_elapsedTime += dt;
	while (m_elapsedTime >= m_updateTime) {
		m_elapsedTime -= m_updateTime;
		UpdateParticles();
	}
}

void ParticleEmitter::AddParticles() {
	for (int i = m_particles.size(); i < m_max; i++) {
		float randX = Random::RandFloat(-m_spread, m_spread);
		float randY = Random::RandFloat(-m_spread, m_spread);

		// god please
		if (m_direction == sf::Vector2f()) {
			randX = Random::RandInt(0, 1) ? Random::RandFloat(-1.15f, -0.15f) : Random::RandFloat(0.15f, 1.15f);
			randY = Random::RandInt(0, 1) ? Random::RandFloat(-1.15f, -0.15f) : Random::RandFloat(0.15f, 1.15f);
		}

		sf::Vector2f dir = sf::Vector2f
		(
			m_direction.x == 0.0f ? randX : m_direction.x * Random::RandFloat(0.90f, 1.12f),
			m_direction.y == 0.0f ? randY : m_direction.y * Random::RandFloat(0.90f, 1.12f)
		);

		m_particles.push_back(Particle(Random::RandFloat(m_lifeTimeMin, m_lifeTimeMax), dir));

		AddQuad(m_position);
		SetQuadRotation(i, Random::RandInt(0, 180));
	}

}

void ParticleEmitter::SetPosition(const sf::Vector2f& pos) {
	m_position = pos;
}

void ParticleEmitter::SetDirection(const sf::Vector2f& dir) {
	m_direction = dir;
}

void ParticleEmitter::SetLifeTimeRange(const float& min, const float& max) {
	m_lifeTimeMin = min;
	m_lifeTimeMax = max;
}

void ParticleEmitter::SetSpeed(const float& speed) {
	m_movementSpeed = speed;
}
void ParticleEmitter::SetBirthColor(sf::Color color) {
	m_birthColor = sf::Vector3f(color.r, color.g, color.b);
}
void ParticleEmitter::SetDeathColor(sf::Color color) {
	m_deathColor = sf::Vector3f(color.r, color.g, color.b);
}

void ParticleEmitter::SetParticleMax(unsigned max) {
	m_max = max;
}

void ParticleEmitter::SetSpread(float spraed) {
	m_spread = spraed;
}

void ParticleEmitter::SetSize(unsigned size) {
	m_size = size;
}

void ParticleEmitter::Clear() {
	m_particles.clear();
	m_vertices.clear();
}


void ParticleEmitter::UpdateParticles() {
	for (int i = 0; i < m_particles.size(); i++) {
		auto& particle = m_particles[i];

		particle.lifeTime -= 0.1f;

		if (particle.lifeTime <= 0.0f) {
			DeleteQuad(i);
			continue;
		}

		float life = particle.lifeTime / particle.constLifeTime;
		
		sf::Vector3f color = Lerp(m_deathColor, m_birthColor, life);

		SetQuadColor(i, sf::Color(color.x, color.y, color.z, life * 255));

		float scale = Lerp((96.0f * m_size) / 100.0f, m_size, life); // i should consider making function for set this shit with now is (96.0f * m_size) / 100.0f

		SetQuadScale(i, scale / m_size);

		MoveQuad(i, particle.direction);
	}
}

void ParticleEmitter::AddQuad(const sf::Vector2f pos) {
	std::array<sf::Vertex, 4> vertices;

	vertices[0].position = sf::Vector2f(pos.x,			pos.y);
	vertices[1].position = sf::Vector2f(pos.x + m_size, pos.y);
	vertices[2].position = sf::Vector2f(pos.x + m_size, pos.y + m_size);
	vertices[3].position = sf::Vector2f(pos.x,			pos.y + m_size);

	for (auto& v : vertices) {
		v.color = sf::Color(m_birthColor.x, m_birthColor.y, m_birthColor.z);
		m_vertices.push_back(v);
	}
}

void ParticleEmitter::DeleteQuad(unsigned index) {
	m_particles.erase(m_particles.begin() + index);

	index *= 4;
	for (int i = index; i < index + 4; i++)
		m_vertices.erase(m_vertices.begin() + index);
}

void ParticleEmitter::SetQuadColor(unsigned index, const sf::Color& color) {
	index *= 4;

	for (int i = index; i < index + 4; i++)
		m_vertices[i].color = color;
}

void ParticleEmitter::SetQuadScale(unsigned index, const float& scale) {
	// yeah i think should consider suicide after writing this function

	index *= 4;

	float sizeX = (m_vertices[index].position.x - m_vertices[index + 1].position.x) / 2.0f;
	float sizeY = (m_vertices[index].position.y - m_vertices[index + 3].position.y) / 2.0f;

	float centX = m_vertices[index].position.x - sizeX;
	float centY = m_vertices[index].position.y - sizeY;

	sf::Transform t;
	t.scale(sf::Vector2f(scale, scale), sf::Vector2f(centX, centY));

	for (int i = index; i < index + 4; i++)
		m_vertices[i].position = t.transformPoint(m_vertices[i].position);
}

void ParticleEmitter::SetQuadRotation(unsigned index, const float& angle) {
	index *= 4;

	float sizeX = (m_vertices[index].position.x - m_vertices[index + 1].position.x) / 2.0f;
	float sizeY = (m_vertices[index].position.y - m_vertices[index + 3].position.y) / 2.0f;

	float centX = m_vertices[index].position.x - sizeX;
	float centY = m_vertices[index].position.y - sizeY;

	sf::Transform t;
	t.rotate(angle, sf::Vector2f(centX, centY));

	for (int i = index; i < index + 4; i++) 
		m_vertices[i].position = t.transformPoint(m_vertices[i].position);
}

void ParticleEmitter::MoveQuad(unsigned index, const sf::Vector2f& dir) {
	index *= 4;

	const sf::Vector2f velocity = sf::Vector2f(m_movementSpeed * dir.x, m_movementSpeed * dir.y);
	sf::Transform t;
	t.translate(velocity);

	for (int i = index; i < index + 4; i++)
		m_vertices[i].position = t.transformPoint(m_vertices[i].position);
}

sf::Vector3f ParticleEmitter::Lerp(const sf::Vector3f& x, const sf::Vector3f& y, const float& t) {
	return x * (1.f - t) + y * t;
}

float ParticleEmitter::Lerp(const float& x, const float& y, const float& t) {
	return x * (1.f - t) + y * t;
}

void ParticleEmitter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_vertices.data(), m_vertices.size(), sf::Quads);
}
