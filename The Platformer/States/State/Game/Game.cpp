#include "stdafx.h"
#include "Game.hpp"

#include "../States/State/Pause/Pause.hpp"
#include "../States/State/Menu/Menu.hpp"
#include "../Effects/Camera/Camera.hpp"
#include "Key Check.hpp"

#include "Random.hpp"

Game::Game(StateMachine& machine)
	: State(machine)
{
	InitAssets();
	InitEntities();
	InitWalls();
	InitSprites();
	InitLights();
	InitCountdown();
	InitTexts();
	InitTimers();

	MusicController::Get().GetMusic("main_track").play();
	MusicController::Get().GetMusic("main_track").setLoop(true);
}

Game::~Game() {
	for (auto& f : m_fireballs)
		delete f;

	delete m_heart;

	delete m_countdown;
	delete m_player;
}

void Game::FixedUpdate() {
	if (m_player->IsAlive())
		FixedUpdateEntities();
}

void Game::Update() {
	// AVENTUSSSSS SUSSSSS AMOGUSSSSSSSSS
	if (m_player->IsAlive()) {
		UpdateEntities();
		m_bestTime = m_timeClock.getElapsedTime().asSeconds();
	}
	else {
		Transition::Get().SetFunction([&]() {
			if (m_bestTime > Extern::best_time)
				Extern::best_time = m_bestTime;
			i_machine.ClearAndPush(new Menu(i_machine));
			MusicController::Get().GetMusic("main_track").stop();
			Transition::Get().Start(Mode::Unveil);
		});
		Transition::Get().Start(Mode::Veil);
	}

	UpdateTexts();
	UpdateCountdown();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && KeyCheck::Key('/')) { // '/' <- is escape keyy XDDDD
		i_machine.AddStateAtTop(new Pause(i_machine));
		MusicController::Get().GetMusic("main_track").setPitch(0.79f);
	}
}

void Game::Render(sf::RenderTarget& target) {
	target.draw(m_Sprites["background"]);

	target.draw(*m_player);

	auto& fog = ShaderLibrary::Get().GetShader("fog");
	fog.setUniform("u_time", m_clock.getElapsedTime().asSeconds());

	target.draw(m_Sprites["foreground"]);

	auto& light = ShaderLibrary::Get().GetShader("light");
	light.setUniform("u_time", m_clock.getElapsedTime().asSeconds());

	light.setUniform("color", sf::Glsl::Vec4(0.75, 0.42, 0.28, 0.72));
	for (const auto& f : m_fireballs)
		target.draw(*f);

	target.draw(*m_countdown);

	if(m_countdown->CurrentFrame() >= 4) //god
		for (const auto& t : m_Texts)
			target.draw(t.second);

	for (const auto& g : m_ghosts)
		target.draw(*g);

	if(m_heart)
		target.draw(*m_heart);

	target.draw(m_fog, &fog);

	light.setUniform("color", sf::Glsl::Vec4(0.65, 0.50, 0.28, 0.8));
	for(const auto& l : m_lights)
		target.draw(l);

}

void Game::SpawnHeart() {
	if (m_heartSpawnClock.getElapsedTime().asSeconds() > 27.5f)
		m_heart = new Heart(&m_TextureManager.Get("heart"), i_dt, i_fdt);
}

void Game::FixedUpdateEntities() {
	m_player->FixedUpdate();
	if (m_heart)
		m_heart->FixedUpdate();
	
	for (auto& g : m_ghosts)
		g->FixedUpdate();

	for (auto& f : m_fireballs)
		f->FixedUpdate();
}

void Game::UpdateEntities() {
	m_player->ResolveCollision(m_walls);
	if (m_heart)
		m_player->ResolveCollision(m_heart);

	m_player->Update();

	if (!m_heart)
		SpawnHeart();

	if (m_heart && !m_heart->IsAlive()) {
		delete m_heart;
		m_heart = nullptr;
	}
	else if (m_heart) {
		m_heart->Update();
		m_heart->ResolveCollision(m_walls);

		m_heartSpawnClock.restart();
	}

	// Fireballs

	for (int i = 0; i < m_fireballs.size(); i++) {
		auto& fireball = m_fireballs[i];

		if (!fireball->Alive()) {
			delete fireball;
			m_fireballs.erase(m_fireballs.begin() + i);

			continue;
		}

		m_player->ResolveCollision(fireball);

		fireball->Update();
	}

	// Ghosts
	for (int i = 0; i < m_ghosts.size(); i++) {
		auto& ghost = m_ghosts[i];

		if (!ghost->m_isAlive) {
			delete ghost;
			m_ghosts.erase(m_ghosts.begin() + i);

			continue;
		}

		m_player->ResolveCollision(ghost, m_ghosts);

		ghost->Update(m_player->GetCollider().GetBody());
	}

	if (m_countdown->CurrentFrame() >= 4)
		UpdateTimers();
}

void Game::InitEntities() {
	//PLAYER//
	{
		m_player = new Player(i_dt, i_fdt, &m_TextureManager.Get("player"));
	}

	//FOG//
	{
		m_fog.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	}
}

void Game::UpdateTimers() {
	//LOG(m_enemySpawnTimer.GetUpdateTime(), '\n');
	m_enemySpawnTimer.Update(i_dt);
	m_ghostSpawnTimer.Update(i_dt);
	m_gameSpeedTimer.Update(i_dt);
}

void Game::InitTimers() {
	m_enemySpawnTimer.SetFunction(1.0f, [&]() {
		if (m_fireballs.size() > 8)
			return;

		const float velocity = 450.0f * Random::RandFloat(1.01f, 1.82f);

		m_fireballs.push_back
		(
			new Fireball
			(
				i_dt, i_fdt, &m_TextureManager.Get("fireball"), 
				&m_TextureManager.Get("blow_up"),
				velocity, 
				Random::RandInt(0, 1)
			)
		);

	});

	m_gameSpeedTimer.SetFunction(5.5f, [&]() {
		const float sub   = Random::RandFloat(0.085f, 0.125f);
		const float uTime = m_enemySpawnTimer.GetUpdateTime() - sub;

		m_enemySpawnTimer.SetUpdateTime
		(
			uTime < 0.285f ? 0.285f : uTime
		);

		m_ghostSpawnTimer.SetUpdateTime(m_ghostSpawnTimer.GetUpdateTime() - sub < 7.185f ? 7.185f : m_ghostSpawnTimer.GetUpdateTime() - sub);
	});

	m_ghostSpawnTimer.SetFunction(8.5f, [&]() {
		if (m_ghosts.size() > 2)
			return;

		m_ghosts.push_back(new Ghost(&m_TextureManager.Get("ghost"), i_dt, i_fdt));
	});
}

void Game::UpdateCountdown() {
	if (m_countdown->CurrentFrame() < 4) {
		m_countdown->Update();
		m_timeClock.restart();
	}
}

void Game::InitCountdown() {
	m_countdown = new Countdown(&m_FontManager.Get("default"));
}

void Game::UpdateTexts() {
	int n = (int)m_timeClock.getElapsedTime().asSeconds();
	int count = 0;
	while (n != 0) { // AMAOGUSUGUASGF
		n = n / 10;
		count++;
	}

	std::stringstream ss;
	ss.precision(count + 2);
	ss << m_timeClock.getElapsedTime().asSeconds();

	m_Texts["time"].setString(ss.str());
	if(!(count % 10) && count != 0)
		m_Texts["time"].reposition();
}

void Game::InitTexts() {
	m_Texts["time"].setString("0.00s");
	m_Texts["time"].setCharacterSize(90);

	for (auto& t : m_Texts)
		t.second.setFont(m_FontManager.Get("default"));

	m_Texts["time"].setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 65.0f));
}

void Game::InitLights() {
	m_lights.push_back(Light(sf::Vector2f(460.0f, 490.0f),  275.0f));
	m_lights.push_back(Light(sf::Vector2f(210.0f, 200.0f),  275.0f));
	m_lights.push_back(Light(sf::Vector2f(1125.0f, 395.0f), 275.0f));
	m_lights.push_back(Light(sf::Vector2f(1350.0f, 780.0f), 275.0f));
}

void Game::InitSprites() {
	//FOREGROUND//
	{
		auto& s = m_Sprites["foreground"];
		s.setTexture(m_TextureManager.Get("foreground"));
		s.setScale(sf::Vector2f(2.0f, 2.0f));
		s.setPosition(sf::Vector2f(0.0f, 4.0f));
	}

	//BACKGROUND//
	{
		auto& s = m_Sprites["background"];
		s.setTexture(m_TextureManager.Get("background"));
		s.setScale(sf::Vector2f(6.0f, 6.0f));
		s.setPosition(sf::Vector2f(0.0f, 4.0f));
	}
}

void Game::InitWalls() {
	m_walls.push_back(Wall(sf::Vector2f(WINDOW_WIDTH / 2.0f, 855.0f), sf::Vector2f(WINDOW_WIDTH, 100.0f)));
	m_walls.push_back(Wall(sf::Vector2f(800.0f,  772.0f), sf::Vector2f(64.0f,  64.0f)));
	m_walls.push_back(Wall(sf::Vector2f(640.0f,  660.0f), sf::Vector2f(128.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(1072.0f, 660.0f), sf::Vector2f(96.0f,  32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(192.0f,  596.0f), sf::Vector2f(192.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(435.0f,  530.0f), sf::Vector2f(160.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(880.0f,  530.0f), sf::Vector2f(224.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(658.0f,  436.0f), sf::Vector2f(96.0f,  32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(80.0f,   466.0f), sf::Vector2f(96.0f,  32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(1182.0f, 436.0f), sf::Vector2f(192.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(398.0f,  340.0f), sf::Vector2f(224.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(815.0f,  340.0f), sf::Vector2f(96.0f,  32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(1377.0f, 340.0f), sf::Vector2f(128.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(176.0f,  244.0f), sf::Vector2f(160.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(960.0f,  244.0f), sf::Vector2f(192.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(1230.0f, 180.0f), sf::Vector2f(160.0f, 32.0f)));
	m_walls.push_back(Wall(sf::Vector2f(1490.0f, 116.0f), sf::Vector2f(160.0f, 32.0f)));
}

void Game::InitAssets() {
	//TEXTURES//
	{
		m_TextureManager.Load("player", "res/textures/player.png");
		m_TextureManager.Load("background", "res/textures/background.png");
		m_TextureManager.Load("foreground", "res/textures/map.png");

		m_TextureManager.Load("fireball", "res/textures/fireball.png");
		m_TextureManager.Load("blow_up", "res/textures/explo.png");

		m_TextureManager.Load("heart", "res/textures/heart.png");

		m_TextureManager.Load("ghost", "res/textures/ghost.png");
	}

	//FONTS//
	{
		m_FontManager.Load("default", "res/fonts/upheavtt.ttf");
	}
}
