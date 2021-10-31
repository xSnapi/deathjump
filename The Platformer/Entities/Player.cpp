#include "stdafx.h"
#include "Player.hpp"

#include "Key Check.hpp"

#include "../Public Variables/Extern.hpp"
#include "../Effects/Music controller/Music Controller.hpp"
#include "../Effects/Camera/Camera.hpp"

#include "../Entities/Fireball.hpp"

Player::Player
(
	const float& dt, 
	const float& fdt,
	sf::Texture* texture
) 
	: Entity(dt, fdt)
{
	i_texture = texture;

	InitBody();
	InitCollider();
	InitAnimations();
	InitEmitters();
	
	m_ghostBuffer.loadFromFile("res/sounds/ghost.wav");
	m_ghost.setBuffer(m_ghostBuffer);
}

Player::~Player() {
	delete m_emitter;
	delete m_fallEmitter;
}

void Player::FixedUpdate() {
	Move();
}

void Player::Update() {
	Animate();
	UpdateVelocity();
	KeepInBorders();
	UpdateEmitters();
	UpdateTimer();

	// this here oh god
	if (m_healthBar.GetHealthState() <= 0)
		m_alive = false;
}

void Player::ResolveCollision(Entity* entity) {
	sf::Vector2f MTV;

	if (!GetCollider().CheckCollision(entity->GetCollider(), MTV) || m_hit)
		return;

	auto fireball = reinterpret_cast<Fireball*>(entity);

	// i know do not scream
	fireball->m_blowUp = true; 
	fireball->m_emitter->Clear();
	fireball->m_emitter->SetDirection(sf::Vector2f(0, 0));
	fireball->m_emitter->SetSpeed(5.4f);
	fireball->m_emitter->SetParticleMax(80);
	fireball->m_emitter->SetSize(25);
	fireball->m_emitter->SetLifeTimeRange(1.5f, 3.2f);
	fireball->m_emitter->AddParticles();

	m_healthBar.Change(true);

	MusicController::Get().GetSound("blow_up").play();
	Camera::Get().Shake(1.75f);

	m_hit		 = true;
	m_grabbing   = false;
	m_movable	 = false;
	m_torque     = 0.925f;

	//m_Animations["takedamage"].SetFrame(0);

	m_velocity.y = -950.0f;
	int multi = (reinterpret_cast<Fireball*>(entity)->m_left ? 1 : -1);
	m_velocity.x = multi * 1200.0f;

	reinterpret_cast<sf::Sprite*>(i_drawable)->setScale(sf::Vector2f(2.0f * -multi, 2.0f));
}

void Player::ResolveCollision(Ghost* entity, std::vector<Ghost*>& ghosts) {
	sf::Vector2f MTV;

	if (!GetCollider().CheckCollision(entity->GetCollider(), MTV) || m_hit || entity->m_blowUp)
		return;

	m_ghost.setVolume(Extern::sound_volume);
	m_ghost.play();

	if (MTV.y < 0.0f && m_velocity.y != 0.0f) {
		// i know do not scream
		entity->m_blowUp = true;
		entity->m_emitter->Clear();
		entity->m_emitter->SetDirection(sf::Vector2f(0, 0));
		entity->m_emitter->SetSpeed(5.4f);
		entity->m_emitter->SetParticleMax(80);
		entity->m_emitter->SetSize(25);
		entity->m_emitter->SetLifeTimeRange(1.5f, 3.2f);
		entity->m_emitter->AddParticles();

		entity->m_animator.SetFrame(0);
		entity->m_animator.SetUpdateTime(0.1f);

		MusicController::Get().GetSound("ghost").play();

		m_velocity.y = m_jumpVelocity + 400.0f;
		m_Animations["jump"].SetFrame(0);

		MusicController::Get().GetSound("player_jump").play();
		return;
	}


	// i know do not scream
	for (auto& g : ghosts) {
		g->m_blowUp = true;
		g->m_emitter->Clear();
		g->m_emitter->SetDirection(sf::Vector2f(0, 0));
		g->m_emitter->SetSpeed(5.4f);
		g->m_emitter->SetParticleMax(80);
		g->m_emitter->SetSize(25);
		g->m_emitter->SetLifeTimeRange(1.5f, 3.2f);
		g->m_emitter->AddParticles();

		g->m_animator.SetFrame(0);
		g->m_animator.SetUpdateTime(0.1f);
	}

	m_healthBar.Change(true);

	MusicController::Get().GetSound("blow_up").play();
	MusicController::Get().GetSound("ghost").play();
	Camera::Get().Shake(1.75f);

	m_hit = true;
	m_grabbing = false;
	m_movable = false;
	m_torque = 0.925f;

	//m_Animations["takedamage"].SetFrame(0);

	m_velocity.y = -950.0f;
	int multi = entity->m_left ? 1 : -1;
	m_velocity.x = multi * 1200.0f;

	reinterpret_cast<sf::Sprite*>(i_drawable)->setScale(sf::Vector2f(2.0f * -multi, 2.0f));
}

void Player::ResolveCollision(Heart* entity) {
	sf::Vector2f MTV;

	if (GetCollider().CheckCollision(entity->GetCollider(), MTV) && m_healthBar.GetHealthState() != 3) {
		m_healthBar.Change(false);
		MusicController::Get().GetSound("pick_up").play();
		entity->_IsAlive = false;
	}
}

void Player::ResolveCollision(std::vector<Wall>& walls) {
	sf::Vector2f MTV;

	m_grounded  = false;

	auto body = reinterpret_cast<sf::Sprite*>(i_drawable);

	for (auto& o : walls) {
		if (!GetCollider().CheckCollision(o.GetCollider(), MTV))
			continue;

		if (MTV.y < 0.0f && m_velocity.y >= 0.0f) {
			m_grounded  = true;
			m_velocity.y = 0.0f;
			Crouch();
		}
		else if (MTV.y > 0.0f && m_velocity.y < 0.0f) {
			m_velocity.y = 0.0f;
		}

		i_collider.position += MTV;
		body->setPosition(i_collider.position);

		if (MTV.x != 0.0f && m_velocity.y > 0.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_movable) {
			bool diffY = i_collider.position.y - i_collider.size.y / 2.0f > o.GetCollider().GetBody().position.y - o.GetCollider().GetBody().size.y;

			if (!diffY)
				return;

			m_grabbing = true;

			m_velocity.y = 0.0f;
			m_velocity.x = 0.0f;

			bool diffX = body->getPosition().x > o.GetCollider().GetBody().position.x + o.GetCollider().GetBody().size.x / 2.0f;

			const int mulit = (diffX) ? -1 : 1;

			body->setScale(sf::Vector2f(2.0f * mulit, 2.0f));
			i_collider.position.y += -(i_collider.position.y - i_collider.size.y / 2.0f - o.GetCollider().GetBody().position.y + o.GetCollider().GetBody().size.y / 2.0f);
		}

		MTV = sf::Vector2f();
	}

	if (m_velocity.y > 0.0f)
		m_wasGrounded = false;
}

bool Player::IsAlive() const {
	return m_alive;
}

void Player::UpdateTimer() {
	if (m_hit) {
		if (m_currentHitTake > 9) {
			m_currentHitTake = 0;
			m_hit = false;
			return;
		}

		unsigned alpha[2] =
		{
			60,
			255,
		};

		auto body = reinterpret_cast<sf::Sprite*>(i_drawable);
		
		sf::Color c = body->getColor();

		body->setColor(sf::Color(c.r, c.g, c.b, alpha[(!(m_currentHitTake % 2) ? 0 : 1)]));

		if (m_hitTimer.getElapsedTime().asSeconds() > 0.175f) {
			m_currentHitTake++;
			m_hitTimer.restart();
		}
	}
}

void Player::Animate() {
	if (!m_movable) {
		m_Animations["takedamage"].Update(i_dt);
		if (m_Animations["takedamage"].GetCurrentFrame() == m_Animations["takedamage"].GetFrameCount() - 1) {
			m_Animations["takedamage"].SetFrame(0);
			m_movable = true;
			m_torque  = 0.85f;
		}
		return;
	}

	if(m_grabbing) {
		m_Animations["grab"].Update(i_dt);
		return;
	}

	if (m_crouching) {
		m_Animations["crouch"].Update(i_dt);
		return;
	}

	if (m_velocity.y == 0.0f) { // NOT JUMPING
		if (m_velocity.x < 0) {
			m_Animations["move"].Update(i_dt);
		}
		else if (m_velocity.x > 0) {
			m_Animations["move"].Update(i_dt);
		}
		else if (m_velocity.x == 0) {
			m_Animations["idle"].Update(i_dt);
		}
	}
	else {
		if (m_velocity.y < 0) {
			if (m_Animations["jump"].GetCurrentFrame() != m_Animations["jump"].GetFrameCount() - 1)
				m_Animations["jump"].Update(i_dt);
		}
		if (m_velocity.y > 0) {
			m_Animations["fall"].Update(i_dt);
		}
	}
}

void Player::KeepInBorders() {
	if (i_collider.position.x < 0.0f)
		i_collider.position.x = 0.0f;
	else if (i_collider.position.x > WINDOW_WIDTH)
		i_collider.position.x = WINDOW_WIDTH;
}

void Player::Crouch() {
	bool wasCrouching = m_crouching;
	// yeah i know
	m_crouching = sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_grounded && m_movable;

	if (m_crouching) {
		//float preY = i_collider.position.y;

		m_velocity.x = 0.0f;
		i_collider.size.y = 40.0f;
		
		if (!wasCrouching) 
			i_collider.position.y += 20.0f;

		reinterpret_cast<sf::Sprite*>(i_drawable)->setOrigin(sf::Vector2f(m_playerSize.x / 2, m_playerSize.y / 1.135f));
	}
	else {
		i_collider.size.y = 80.0f;
		if (wasCrouching)
			i_collider.position.y -= 20.0f;
	}

	if (wasCrouching != m_crouching && m_crouching)
		MusicController::Get().GetSound("player_crouch").play();
}

void Player::Move() {
	if(!m_grabbing)
		m_velocity.y += m_gravity * i_fdt;
	m_velocity.x *= m_torque;
	i_collider.position += m_velocity * i_fdt;

	float vel = abs(m_velocity.x);
	m_velocity.x = (vel < 8.0f ? 0.0f : m_velocity.x);

	auto body = reinterpret_cast<sf::Sprite*>(i_drawable);

	body->setOrigin(sf::Vector2f(m_playerSize.x / 2, m_playerSize.y / 1.315f));

	if(m_crouching)
		body->setOrigin(sf::Vector2f(m_playerSize.x / 2, m_playerSize.y / 1.135f));

	body->setPosition((m_grabbing ? sf::Vector2f(i_collider.position.x + 5.0f * body->getScale().x / 2.0f, i_collider.position.y) : i_collider.position));
}

void Player::UpdateVelocity() {
	if (m_crouching || !m_movable)
		return;

	if (
			m_grounded &&
			sf::Keyboard::isKeyPressed(sf::Keyboard::W) && 
			KeyCheck::Key('W') || 
			m_grabbing && 
			sf::Keyboard::isKeyPressed(sf::Keyboard::W) && 
			KeyCheck::Key('W')
		) 
	{
		m_grounded = false;
		m_grabbing = false;
		m_velocity.y = m_jumpVelocity;
		m_Animations["jump"].SetFrame(0);

		MusicController::Get().GetSound("player_jump").play();
	}

	if (m_grabbing && !sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_grabbing = false;

	if (m_grabbing)
		return;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_velocity.x = -m_movementSpeed;
		reinterpret_cast<sf::Sprite*>(i_drawable)->setScale(sf::Vector2f(-2.0f, 2.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_velocity.x = m_movementSpeed;
		reinterpret_cast<sf::Sprite*>(i_drawable)->setScale(sf::Vector2f(2.0f, 2.0f));
	}
}

void Player::UpdateEmitters() {
	m_emitter->SetDirection(sf::Vector2f((m_velocity.x < 0.0f ? 1.0f : -1.0f), 0.0f));

	m_emitter->SetPosition(sf::Vector2f(i_collider.position.x, i_collider.position.y - 10.0f));
	if (m_velocity.x != 0.0f)
		m_emitter->AddParticles();
	m_emitter->Update(i_dt);

	m_emitter->SetBirthColor(Extern::particle_birth_color);
	m_emitter->SetDeathColor(Extern::particle_death_color);

	m_fallEmitter->SetPosition(sf::Vector2f(i_collider.position.x - 10.0f, i_collider.position.y + 32.0f));
	if (!m_wasGrounded && m_grounded) {
		m_fallEmitter->AddParticles();
		m_wasGrounded = true;
	}
	m_fallEmitter->Update(i_dt);
}

void Player::InitEmitters() {
	{
		m_emitter = new ParticleEmitter(Extern::particle_birth_color, Extern::particle_death_color, 15);
		m_emitter->SetLifeTimeRange(0.5f, 2.0f);
		m_emitter->SetSpeed(1.5f);
	}

	{
		m_fallEmitter = new ParticleEmitter(sf::Color(79, 46, 26), sf::Color(20, 48, 18), 13);
		m_fallEmitter->SetLifeTimeRange(0.5f, 1.5f);
		m_fallEmitter->SetDirection(sf::Vector2f(0, -1));
		m_fallEmitter->SetSpread(4.59f);
		m_fallEmitter->SetSpeed(1.1f);
		m_fallEmitter->SetParticleMax(35);
	}
}

void Player::InitAnimations() {
	auto sprite = reinterpret_cast<sf::Sprite*>(i_drawable);

	m_Animations["move"].		Create(	  8, 0, 0.08f,  m_playerSize, *sprite, 3);
	m_Animations["idle"].		Create(	  7, 0, 0.08f,  m_playerSize, *sprite, 1);
	m_Animations["jump"].		Create(	  2, 0, 0.125f, m_playerSize, *sprite, 4);
	m_Animations["fall"].		Create(	  1, 0, 0.1f,   m_playerSize, *sprite, 6);
	m_Animations["crouch"].		Create(	  6, 0, 0.06f,  m_playerSize, *sprite, 9);
	m_Animations["grab"].		Create(	  1, 0, 0.1f,   m_playerSize, *sprite, 15);
	m_Animations["takedamage"]. Create(	  7, 0, 0.08f,  m_playerSize, *sprite, 17);
}

void Player::InitBody() {
	i_drawable = new sf::Sprite(*i_texture);

	auto sprite = reinterpret_cast<sf::Sprite*>(i_drawable);

	sprite->setOrigin(sf::Vector2f(m_playerSize.x / 2, m_playerSize.y / 1.315f));
	sprite->setScale(sf::Vector2f(2.f, 2.f));
}

void Player::InitCollider() {
	const sf::Vector2f size = sf::Vector2f(40.0f, 80.0f);

	i_collider.position = sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT) / 2.0f;
	i_collider.size = size;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*m_emitter, i_shader);
	target.draw(*m_fallEmitter, i_shader);
	target.draw(*i_drawable, i_shader);
	target.draw(m_healthBar);
	/*sf::RectangleShape rs;
	rs.setSize(i_collider.size);
	rs.setOrigin(i_collider.size / 2.0f);
	rs.setPosition(i_collider.position);
	target.draw(rs, i_shader);*/
}
