#pragma once
#include "stdafx.h"

#include "../Public Variables/Extern.hpp"

class MusicController {
public:
	MusicController(const MusicController&) = delete;

	static MusicController& Get();

	void Update();

	void AddSound(std::string name, std::string path);
	void AddMusic(std::string name, std::string path);

	sf::Sound& GetSound(std::string name);
	sf::Music& GetMusic(std::string name);
private:
	MusicController() {}
	~MusicController() {}

	std::vector<sf::SoundBuffer> m_Buffers;
	std::unordered_map<std::string, sf::Music> m_Musics;
	std::unordered_map<std::string, sf::Sound> m_Sounds;

	static MusicController s_instance;
};