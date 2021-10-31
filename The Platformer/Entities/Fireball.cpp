#include "stdafx.h"
#include "Fireball.hpp"

#include "Random.hpp"

Fireball::Fireball
(
	const float& dt, 
	const float& fdt, 
	sf::Texture* texture,
	sf::Texture* blowUp, 
	float velocity, 
	bool left
) 
	: Entity(dt, fdt), m_velocity(velocity), m_dt(dt), m_fdt(fdt) // haha ;))))) i wanna killmyselfffffff
{
	i_texture = texture;
	m_blowTexture = blowUp;
	m_left = left;

	InitCollider(sf::Vector2f(left ? -100.f : WINDOW_WIDTH + 100.f, m_positions[Random::RandInt(0, 12)]));
	InitSprites();
	InitAnimations();
	InitEmiiter();
	InitLight();
}

Fireball::~Fireball() {
	delete m_emitter;
	delete m_light;
}

void Fireball::FixedUpdate() {
	if(!m_blowUp)
		Move();
}

void Fireball::Update() {
	UpdateLight();
	UpdateEmitter();
	Animate();

	if (i_collider.position.x > WINDOW_WIDTH + 300.0f || i_collider.position.x < -300.0f)
		m_alive = false;
}

bool Fireball::Alive() const {
	return m_alive;
}

void Fireball::Move() {
	int multi = (m_left ? 1 : -1);

	i_collider.position += sf::Vector2f(m_velocity * multi * m_fdt, 0.0f);
	m_fireballSprite.setPosition(i_collider.position);
}

void Fireball::UpdateLight() {
	m_light->SetPosition(i_collider.position);
}

void Fireball::InitLight() {
	m_light = new Light(i_collider.position, 290.0f);
}

void Fireball::UpdateEmitter() {
	if(!m_blowUp)
		m_emitter->AddParticles();
	m_emitter->Update(m_dt);
	m_emitter->SetPosition(i_collider.position);
}

void Fireball::InitEmiiter() {
	m_emitter = new ParticleEmitter(sf::Color(255, 255, 0), sf::Color(255, 0, 0), 20);
	m_emitter->SetDirection(sf::Vector2f(m_left ? -1 : 1, 0.0f));
	m_emitter->SetLifeTimeRange(0.8f, 2.0f);
	m_emitter->SetSpeed(4.1f);
	m_emitter->SetPosition(i_collider.position);
}

void Fireball::Animate() {
	if (m_blowUp) {

		if (m_Animations["blow_up"].GetCurrentFrame() == m_Animations["blow_up"].GetFrameCount() - 1) {
			m_alive = false;
			return;
		}

		m_blowUpSprite.setPosition(i_collider.position);
		m_Animations["blow_up"].Update(m_dt);
	}
}

void Fireball::InitAnimations() {
	m_Animations["blow_up"].Create(12, 0, 0.05f, sf::Vector2i(512, 512), m_blowUpSprite);
}

void Fireball::InitSprites() {
	{
		m_fireballSprite.setTexture(*i_texture);
		m_fireballSprite.setOrigin(sf::Vector2f(m_size.x, m_size.y) / 2.0f);
		m_fireballSprite.setScale(sf::Vector2f(m_left ? 2.f : -2.f, 2.f));
		m_fireballSprite.setPosition(i_collider.position);
	}

	{
		sf::Vector2f size = sf::Vector2f(512.f, 512.f);

		m_blowUpSprite.setTexture(*m_blowTexture);
		m_blowUpSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
		m_blowUpSprite.setOrigin(size / 2.f);
		m_blowUpSprite.setScale(sf::Vector2f(0.30f, 0.30f));
	}
}

void Fireball::InitCollider(sf::Vector2f position) {
	const sf::Vector2f size = sf::Vector2f(32.f, 32.f);
	i_collider.size = size;
	i_collider.position = position;
}

void Fireball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_emitter);
	if (!m_blowUp)
		target.draw(m_fireballSprite);
	else
		target.draw(m_blowUpSprite);
	target.draw(*m_light);
	/*sf::RectangleShape rs;
	rs.setSize(i_collider.size);
	rs.setOrigin(i_collider.size / 2.0f);
	rs.setPosition(i_collider.position);
	target.draw(rs, i_shader);*/
}
