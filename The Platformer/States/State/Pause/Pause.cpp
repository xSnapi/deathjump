#include "stdafx.h"
#include "Pause.hpp"

#include "../States/State/Menu/Menu.hpp"

#include "Key Check.hpp"

Pause::Pause(StateMachine& machine)
	: State(machine)
{
	InitAssets();
	InitSprites();
	InitTexts();
	InitButtons();
	InitTimer();
}

Pause::~Pause() {

}

void Pause::FixedUpdate() {

}

void Pause::Update() {
	UpdateButtons();
	AnimateTexts();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && KeyCheck::Key('/')) { // '/' <- is escape keyy XDDDD
		MusicController::Get().GetMusic("main_track").setPitch(1.0f);
		i_isRunning = false;
	}
}

void Pause::Render(sf::RenderTarget& target) {
	// Sprites drawing

	auto& shader = ShaderLibrary::Get().GetShader("blur");
	shader.setUniform("u_blur_radius", 0.008f);

	for (const auto& s : m_Sprites)
		target.draw(s.second, &ShaderLibrary::Get().GetShader("blur"));

	// Text drawing
	for (const auto& t : m_Texts)
		target.draw(t.second);

	// Button drawing
	for (const auto& b : m_Buttons)
		target.draw(b.second);
}

void Pause::AnimateTexts() {
	m_textAnimTimer.Update(i_dt);
}

void Pause::InitTimer() {
	m_textAnimTimer.SetFunction(0.20f, [&]() {
		constexpr float pos[10] =
		{
			100.0f,
			110.0f,
			120.0f,
			130.0f,
			140.0f,
			130.0f,
			120.0f,
			110.0f,
			100.0f,
			90.0f,
		};

		m_Texts["title"].setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, pos[m_iterator]));

		m_iterator++;
		if (m_iterator > 9)
			m_iterator = 0;
		});
}

void Pause::UpdateButtons() {
	for (auto& b : m_Buttons)
		b.second.Update(GetMousePos());
}

void Pause::InitButtons() {
	std::initializer_list<std::pair<const std::string, Button>> init =
	{
		{ "resume",		  Button("RESUME",		 sf::Vector2f(WINDOW_WIDTH / 2.0f, 400), m_FontManager.Get("default")) },
		{ "settings",	  Button("SETTINGS",	 sf::Vector2f(WINDOW_WIDTH / 2.0f, 500), m_FontManager.Get("default")) },
		{ "exit_to_menu", Button("EXIT TO MENU", sf::Vector2f(WINDOW_WIDTH / 2.0f, 600), m_FontManager.Get("default")) },
	};

	m_Buttons = init;

	auto& transition = Transition::Get();

	m_Buttons["resume"].SetFunction([&]() {
		MusicController::Get().GetMusic("main_track").setPitch(1.0f);
		i_isRunning = false;
	});

	m_Buttons["settings"].SetFunction([&]() {
		transition.SetFunction([&]() {
			i_machine.AddStateAtTop(new Settings(i_machine));
			transition.Start(Mode::Unveil);
		});

		transition.Start(Mode::Veil);
	});

	m_Buttons["exit_to_menu"].SetFunction([&]() {
		transition.SetFunction([&]() {
			i_machine.ClearAndPush(new Menu(i_machine));
			MusicController::Get().GetMusic("main_track").setPitch(1.0f);
			MusicController::Get().GetMusic("main_track").stop();
			transition.Start(Mode::Unveil);
		});

		transition.Start(Mode::Veil);
	});
}

void Pause::InitSprites() {
	{
		auto& s = m_Sprites["background"];
		s.setTexture(m_TextureManager.Get("background"));
	}
}

void Pause::InitTexts() {
	// TITLE
	{
		auto& t = m_Texts["title"];
		t.setString("PAUSED");
		t.setCharacterSize(200);
	}

	for (auto& t : m_Texts)
		t.second.setFont(m_FontManager.Get("default"));

	m_Texts["title"].setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 120.0f));

}

void Pause::InitAssets() {
	//TEXTURES//
	{
		m_TextureManager.Load("background", "res/textures/menu.png");
	}

	//FONTS//
	{
		m_FontManager.Load("default", "res/fonts/upheavtt.ttf");
	}
}
