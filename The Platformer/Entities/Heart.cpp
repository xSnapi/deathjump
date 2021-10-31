#include "stdafx.h"
#include "Heart.hpp"

#include "Random.hpp"

Heart::Heart
(
	sf::Texture* texture,
	const float& dt,
	const float& fdt
)
	: Entity(dt, fdt)
{
	i_texture = texture;
	InitBody();
	InitAnimations();
	InitLight();
}

Heart::~Heart() {
	delete m_light;
}

void Heart::FixedUpdate() {
	constexpr float gravity = 300.0f;

	if (!m_grounded)
		i_collider.position.y += gravity * i_fdt;

	reinterpret_cast<sf::Sprite*>(i_drawable)->setPosition(i_collider.position);
}

void Heart::Update() {
	m_animation.Update(i_dt);
	UpdateLight();

	if (realDeSpawnClock.getElapsedTime().asSeconds() > deSpawnTime * 3.f)
		_IsAlive = false;


	if (deSpawnClock.getElapsedTime().asSeconds() > deSpawnTime) {
		disappearAnimationTime -= 0.203f;
		disappear = true;
		deSpawnClock.restart();
	}

	if (disappear)
		AnimateDisappear();
}

void Heart::ResolveCollision(std::vector<Wall>& walls) {
	for (auto& o : walls) {
		sf::Vector2f MTV;

		if (!GetCollider().CheckCollision(o.GetCollider(), MTV))
			continue;

		i_collider.position += MTV;
		m_grounded = true;
		break;
	}
}

bool Heart::IsAlive() const {
	return this->_IsAlive;
}

void Heart::UpdateLight() {
	m_light->SetPosition(i_collider.position);
}

void Heart::InitLight() {
	m_light = new Light(i_collider.position, 180.0f);
}

void Heart::AnimateDisappear() {
	if (this->disappearClock.getElapsedTime().asSeconds() > this->disappearAnimationTime) {
		this->currentColor++;
		if (this->currentColor > 4)
			this->currentColor = 0;
		this->disappearClock.restart();
	}

	reinterpret_cast<sf::Sprite*>(i_drawable)->setColor(this->colors[this->currentColor]);
}

void Heart::InitAnimations() {
	m_animation.Create(4, 0, 0.1f, sf::Vector2i(32, 32), *reinterpret_cast<sf::Sprite*>(i_drawable));
}

void Heart::InitBody() {
	int pos = Random::RandInt(32, WINDOW_WIDTH - 32);

	i_drawable = new sf::Sprite(*i_texture);
	reinterpret_cast<sf::Sprite*>(i_drawable)->setOrigin(sf::Vector2f(32.0f, 32.0f) / 2.0f);
	reinterpret_cast<sf::Sprite*>(i_drawable)->setPosition(sf::Vector2f(pos, -100.0f));


	const sf::Vector2f size = sf::Vector2f(32.f, 32.f);

	i_collider.size = size;
	i_collider.position = sf::Vector2f(pos, -100.f);
}

void Heart::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	ShaderLibrary::Get().GetShader("light").setUniform("color", sf::Glsl::Vec4(0.75, 0.20, 0.70, 0.8));
	target.draw(*m_light);
	target.draw(*i_drawable);
}