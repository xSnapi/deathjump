#pragma once
#include "../States/Machine/State Machine.hpp"
#include "Shader Library.hpp"

#include "Timer.hpp"
#include "Text.hpp"

#include "../Entities/Player.hpp"
#include "../Entities/Heart.hpp"
#include "../Entities/Fireball.hpp"
#include "../Entities/Ghost.hpp"
#include "../Objects/Wall.hpp"

#include "../Effects/Light/Light.hpp"

#include "../UI/Countdown.hpp"

class Game : public State {
public:
	Game(StateMachine& machine);
	~Game();

	virtual void FixedUpdate();
	virtual void Update();
	virtual void Render(sf::RenderTarget& target);

private:
	Player* m_player;
	//std::vector<Heart*> m_hearts; // we need to store it in vector because of my brain damage
	Heart* m_heart = nullptr;
	std::vector<Ghost*> m_ghosts;
	
	sf::RectangleShape m_fog;
	sf::Clock m_clock;

	std::unordered_map<std::string, sf::Sprite> m_Sprites;
	std::unordered_map<std::string, Text> m_Texts;

	AssetManager<sf::Texture> m_TextureManager;
	AssetManager<sf::Font> m_FontManager;
	
	std::vector<Wall> m_walls;
	std::vector<Light> m_lights;

	std::vector<Fireball*> m_fireballs;

	sf::Clock m_spawnClock;
	sf::Clock m_timeClock;
	sf::Clock m_heartSpawnClock;
	sf::Clock m_ghostSpawnClock;

	Countdown* m_countdown;

	Timer m_enemySpawnTimer;
	Timer m_ghostSpawnTimer;
	Timer m_gameSpeedTimer;

	float m_bestTime = 0.0f;

	void SpawnHeart();

	void FixedUpdateEntities();
	void UpdateEntities();
	void InitEntities();

	void UpdateTimers();
	void InitTimers();

	void UpdateCountdown();
	void InitCountdown();

	void UpdateTexts();
	void InitTexts();

	void InitLights();

	void InitSprites();

	void InitWalls();

	void InitAssets();
};