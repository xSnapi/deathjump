#include "stdafx.h"
#include "Ghost.hpp"

#include "Random.hpp"

Ghost::Ghost
(
	sf::Texture* texture, 
	const float& dt, 
	const float& fdt
) 
	: Entity(dt, fdt)
{
	i_texture = texture;

	m_left = Random::RandInt(0, 1);

	//m_lifeTime = (float)Random::RandInt(10, 12);

	InitBody();
	InitAnimator();
	InitLight();
	InitEmitter();
}

Ghost::~Ghost() {
	delete m_light;
	delete m_emitter;
}

void Ghost::FixedUpdate() {
	const float velocity = 165.0f;

	i_collider.position += m_dir * velocity * i_fdt;

	i_collider.position.y += 80.0f * (sinf(4 * m_clock.getElapsedTime().asSeconds())) * i_fdt;
		
	m_sprite->setPosition(i_collider.position);

	//LOG(i_collider.position.x, '\n');
}

void Ghost::Update(Collider obj) {
	UpdateLight();

	m_emitter->Update(i_dt);
	m_emitter->SetPosition(i_collider.position);

	if (m_blowUp) {
		m_animator.Update(i_dt);

		if (m_animator.GetCurrentFrame() == m_animator.GetFrameCount() - 1)
			m_isAlive = false;

		return;
	}
	else
		m_emitter->AddParticles();

	m_lifeTime -= i_dt;

	if (m_lifeTime < 2.0f) {
		float life = m_lifeTime / 2.0f;
		float alpha = Lerp(0.0f, 1.0f, life);

		if (alpha <= 0.08f)
			m_isAlive = false;

		if (alpha < 0.55f)
			m_lightVal = alpha;

		m_sprite->setColor(sf::Color(255, 255, 255, alpha * 255));
	}
	obj.position.y += 35.0f;

	sf::Vector2f diff = obj.position - i_collider.position;

	float magnitude = sqrt(diff.x * diff.x + diff.y * diff.y);

	m_dir = diff / magnitude;

	m_left = obj.position.x > i_collider.position.x;

	m_animator.Update(i_dt);
	m_sprite->setScale(m_left ? sf::Vector2f(-2.75f, 2.75f) : sf::Vector2f(2.75f, 2.75f));
}

float Ghost::Lerp(const float& x, const float& y, const float& t) {
	return x * (1.f - t) + y * t;
}

void Ghost::InitEmitter() {
	m_emitter = new ParticleEmitter(sf::Color(255, 255, 255), sf::Color(0, 0, 0), 0);
	m_emitter->SetLifeTimeRange(0.8f, 2.0f);
	m_emitter->SetSpeed(4.1f);
	m_emitter->SetPosition(i_collider.position);
	m_emitter->AddParticles();
}

void Ghost::UpdateLight() {
	m_light->SetPosition(i_collider.position);
}

void Ghost::InitLight() {
	m_light = new Light(sf::Vector2f(), 275.0f);
}

void Ghost::InitAnimator() {
	m_animator.Create(4, 0, 0.1f, sf::Vector2i(64, 64), *m_sprite);
}

void Ghost::InitBody() {
	{
		i_drawable = new sf::Sprite(*i_texture);

		m_sprite = reinterpret_cast<sf::Sprite*>(i_drawable);

		m_sprite->setOrigin(sf::Vector2f(64.0f, 64.0f) / 2.0f);
		m_sprite->setTextureRect(sf::IntRect(0, 0, 64, 64));
	}

	{
		float posX = Random::RandInt(0, 1) ? -100.0f : WINDOW_WIDTH + 100.0f;
		//float posX = left ? 100.0f : 1450.0f;

		i_collider.position = sf::Vector2f(posX, Random::RandInt(90, WINDOW_HEIGHT - 90));
		i_collider.size = sf::Vector2f(10.0f, 13.0f) * 2.75f;
	}
}

void Ghost::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	ShaderLibrary::Get().GetShader("light").setUniform("color", sf::Glsl::Vec4(0.9, 0.9, 0.9, m_lightVal));
	if(!m_blowUp)
		target.draw(*m_sprite);
	target.draw(*m_emitter);
	target.draw(*m_light);
	/*sf::RectangleShape rs;
	rs.setSize(i_collider.size);
	rs.setOrigin(i_collider.size / 2.0f);
	rs.setPosition(i_collider.position);
	target.draw(rs, i_shader);*/
}
