#include "stdafx.h"
#include "Settings.hpp"

Settings::Settings(StateMachine& machine) 
	: State(machine)
{
	InitAssets();
	InitButtons();
	InitSeekerBars();
	InitParticleEmitter();
	InitColorPickers();

	InitSprites();
	InitTexts();
}

Settings::~Settings() {
	for (auto& s : m_SeekerBars)
		delete s.second;

	for (auto& p : m_pickers)
		delete p;

	delete m_emitter;
}

void Settings::FixedUpdate() {

}

void Settings::Update() {
	UpdateButtons();
	UpdateSeekerBars();
	UpdateParticleEmitter();
	UpdateColorPickers();
}

void Settings::Render(sf::RenderTarget& target) {
	auto& shader = ShaderLibrary::Get().GetShader("blur");
	shader.setUniform("u_blur_radius", 0.008f);

	for (const auto& s : m_Sprites)
		target.draw(s.second, &shader);

	for (const auto& b : m_Buttons)
		target.draw(b.second);

	for (const auto& s : m_SeekerBars)
		target.draw(*s.second);

	for (const auto& t : m_Texts)
		target.draw(t.second);

	for (const auto& p : m_pickers)
		target.draw(*p);

	target.draw(*m_emitter);
}

void Settings::UpdateColorPickers() {
	for (auto& p : m_pickers)
		p->Update(GetMousePos());
}

void Settings::InitColorPickers() {
	m_pickers.push_back(new ColorPicker(Extern::particle_birth_color, sf::Vector2f(270.0f, 740.0f)));
	m_pickers.push_back(new ColorPicker(Extern::particle_death_color, sf::Vector2f(480.0f, 740.0f)));
}

void Settings::UpdateParticleEmitter() {
	m_emitter->AddParticles();
	m_emitter->Update(i_dt);
	m_emitter->SetBirthColor(Extern::particle_birth_color);
	m_emitter->SetDeathColor(Extern::particle_death_color);
}

void Settings::InitParticleEmitter() {
	m_emitter = new ParticleEmitter(Extern::particle_birth_color, Extern::particle_death_color, 75);
	m_emitter->SetLifeTimeRange(3.5f, 8.5f);
	m_emitter->SetDirection(sf::Vector2f(1, 0));
	m_emitter->SetPosition(sf::Vector2f(725.0f, 750.0f));
	m_emitter->SetParticleMax(100);
}

void Settings::InitTexts() {
	// TITLE
	{
		auto& t = m_Texts["sound"];
		t.setString("SOUND VOLUME");
		t.setCharacterSize(90);
	}

	{
		auto& t = m_Texts["music"];
		t.setString("MUSIC VOLUME");
		t.setCharacterSize(90);
	}

	{
		auto& t = m_Texts["player_particle"];
		t.setString("PLAYER PARTICLE");
		t.setCharacterSize(90);
	}

	{
		auto& t = m_Texts["primary"];
		t.setString("PRIMARY");
		t.setCharacterSize(40);
	}

	{
		auto& t = m_Texts["secondary"];
		t.setString("SECONDARY");
		t.setCharacterSize(40);
	}

	for (auto& t : m_Texts)
		t.second.setFont(m_FontManager.Get("default"));

	m_Texts["sound"].setPosition(sf::Vector2f(815.0f, 100.0f));
											  
	m_Texts["music"].setPosition(sf::Vector2f(815.0f, 320.0f));

	m_Texts["player_particle"].setPosition(sf::Vector2f(815.0f, 560.0f));

	m_Texts["primary"].setPosition(sf::Vector2f(270.0f, 671.0f));

	m_Texts["secondary"].setPosition(sf::Vector2f(480.0f, 810.0f));
}

void Settings::UpdateSeekerBars() {
	for (auto& s : m_SeekerBars)
		s.second->Update(GetMousePos());
}

void Settings::InitSeekerBars() {
	std::initializer_list<std::pair<const std::string, SeekerBar*>> init =
	{
		{ "sound", new SeekerBar(sf::Vector2f(350.0f, 180.0f), m_FontManager.Get("default"), 10, Extern::sound_volume / 8) },
		{ "music", new SeekerBar(sf::Vector2f(350.0f, 400.0f), m_FontManager.Get("default"), 10, Extern::music_volume / 8) },
	};

	m_SeekerBars = init;

	auto& soundVolume = Extern::sound_volume;

	m_SeekerBars["sound"]->SetLeftFunction([&soundVolume]() {
		if (soundVolume > 0)
			soundVolume -= 8;
	});

	m_SeekerBars["sound"]->SetRightFunction([&soundVolume]() {
		if (soundVolume < 80)
			soundVolume += 8;
	});

	auto& musicVolume = Extern::music_volume;

	m_SeekerBars["music"]->SetLeftFunction([&musicVolume]() {
		if (musicVolume > 0)
			musicVolume -= 8;
	});

	m_SeekerBars["music"]->SetRightFunction([&musicVolume]() {
		if (musicVolume < 80)
			musicVolume += 8;
	});
}

void Settings::InitSprites() {
	{
		auto& s = m_Sprites["background"];
		s.setTexture(m_TextureManager.Get("background"));
	}
}

void Settings::UpdateButtons() {
	for (auto& b : m_Buttons)
		b.second.Update(GetMousePos());
}

void Settings::InitButtons() {
	std::initializer_list<std::pair<const std::string, Button>> init =
	{
		{ "back", Button("BACK", sf::Vector2f(180.0f, 80.0f), m_FontManager.Get("default")) },
	};

	m_Buttons = init;

	auto& transition = Transition::Get();

	m_Buttons["back"].SetFunction([&]() {
		transition.SetFunction([&]() {
			i_isRunning = false; 
			transition.Start(Mode::Unveil);
		});
		transition.Start(Mode::Veil);
	});
}

void Settings::InitAssets() {
	//TEXTURES//
	{
		m_TextureManager.Load("background", "res/textures/menu.png");
	}
	//FONTS//
	{
		m_FontManager.Load("default", "res/fonts/upheavtt.ttf");
	}
}
