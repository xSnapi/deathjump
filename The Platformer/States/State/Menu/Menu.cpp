#include "stdafx.h"
#include "Menu.hpp"

Menu::Menu(StateMachine& machine) 
	: State(machine)
{
	InitAssets();
	InitSprites();
	InitTexts();
	InitButtons();
	InitTimer();
	InitBorder();

	MusicController::Get().GetMusic("menu").play();
	MusicController::Get().GetMusic("menu").setLoop(true);
}

Menu::~Menu() {

}

void Menu::FixedUpdate() {

}

void Menu::Update() {
	UpdateButtons();
	AnimateTexts();
}

void Menu::Render(sf::RenderTarget& target) {
	// Sprites drawing
	for (const auto& s : m_Sprites)
		target.draw(s.second);

	target.draw(m_border);

	// Text drawing
	for (const auto& t : m_Texts)
		target.draw(t.second);

	// Button drawing
	for (const auto& b : m_Buttons)
		target.draw(b.second);
}

void Menu::InitBorder() {
	m_border.setSize(m_Texts["best_time"].getSize() + sf::Vector2f(25.0f, 15.0f));
	m_border.setOrigin((m_Texts["best_time"].getSize() + sf::Vector2f(25.0f, 15.0f)) / 2.0f);
	m_border.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 325.0f));
	m_border.setFillColor(sf::Color(224, 165, 0, 70));
	m_border.setOutlineColor(sf::Color(224, 165, 0));
	m_border.setOutlineThickness(4);
}

void Menu::AnimateTexts() {
	m_textAnimTimer.Update(i_dt);
}

void Menu::InitTimer() {
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

void Menu::UpdateButtons() {
	for (auto& b : m_Buttons)
		b.second.Update(GetMousePos());
}

void Menu::InitButtons() {
	std::initializer_list<std::pair<const std::string, Button>> init =
	{
		{ "start",	  Button("START",	  sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT) / 2.0f, m_FontManager.Get("default")) },
		{ "settings", Button("SETTINGS",  sf::Vector2f(WINDOW_WIDTH / 2.0f, 550   ),        m_FontManager.Get("default")) },
		{ "exit",	  Button("EXIT",	  sf::Vector2f(WINDOW_WIDTH / 2.0f, 650   ),        m_FontManager.Get("default")) },
	};

	m_Buttons = init;

	auto& transition = Transition::Get();

	m_Buttons["start"].SetFunction([&]() {
		transition.SetFunction([&]() {
			i_isRunning = false;
			i_machine.AddStateAtBottom(new Game(i_machine));
			MusicController::Get().GetMusic("menu").stop();
			transition.Start(Mode::Unveil);
		});
			
		transition.Start(Mode::Veil);
	});

	m_Buttons["settings"].SetFunction([&]() { 
		transition.SetFunction([&]() { 
			i_machine.AddStateAtTop(new Settings(i_machine)); 
			transition.Start(Mode::Unveil);  
		});

		transition.Start(Mode::Veil);
	});

	m_Buttons["exit"].SetFunction([&]() { 
		transition.SetFunction([&]() { i_isRunning = false; });
		transition.Start(Mode::Veil);
	});
}

void Menu::InitSprites() {
	{
		auto& s = m_Sprites["background"];
		s.setTexture(m_TextureManager.Get("background"));
	}
}

void Menu::InitTexts() {
	// TITLE
	{
		auto& t = m_Texts["title"];
		t.setString("DEATHJUMP!");
		t.setCharacterSize(200);
	}

	{
		int n = (int)Extern::best_time;
		int count = 0;
		while (n != 0) { // AMAOGUSUGUASGF
			n = n / 10;
			count++;
		}

		std::stringstream ss;
		ss.precision(count + 2);
		ss << Extern::best_time;
		//ss << "s";

		auto& t = m_Texts["best_time"];
		t.setString("BEST TIME: " + ss.str());
		t.setCharacterSize(80);
	}

	for (auto& t : m_Texts) 
		t.second.setFont(m_FontManager.Get("default"));

	m_Texts["title"].setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 120.0f));
	
	m_Texts["best_time"].setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 325.0f));
}

void Menu::InitAssets() {
	//TEXTURES//
	{
		m_TextureManager.Load("background", "res/textures/menu.png");
	}

	//FONTS//
	{
		m_FontManager.Load("default", "res/fonts/upheavtt.ttf");
	}
}
