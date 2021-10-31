#pragma once
#include "../States/Machine/State Machine.hpp"
#include "../States/State/Settings/Settings.hpp"
#include "../States/State/Game/Game.hpp"

#include "Text.hpp"
#include "Timer.hpp"
#include "../UI/Button.hpp"
#include "../Effects/Transition/Transition.hpp"

class Menu : public State {
public:
	Menu(StateMachine& machine);
	~Menu();

	virtual void FixedUpdate();
	virtual void Update();
	virtual void Render(sf::RenderTarget& target);

private:
	AssetManager<sf::Texture> m_TextureManager;
	AssetManager<sf::Font> m_FontManager;

	std::unordered_map<std::string, Text> m_Texts;
	std::unordered_map<std::string, Button> m_Buttons;

	// Sprites
	std::unordered_map<std::string, sf::Sprite> m_Sprites;

	sf::RectangleShape m_border;

	Timer m_textAnimTimer;
	int m_iterator = 0;

	void InitBorder(); // yes

	void AnimateTexts();
	void InitTimer();

	void UpdateButtons();
	void InitButtons();

	void InitSprites();

	void InitTexts();
	void InitAssets();
};