#include "stdafx.h"
#include "Music Controller.hpp"

MusicController& MusicController::Get() {
	return s_instance;
}

void MusicController::Update() {
	for (auto& s : m_Sounds)
		s.second.setVolume((float)Extern::sound_volume);

	for (auto& m : m_Musics)
		m.second.setVolume((float)Extern::music_volume);
}

void MusicController::AddSound(std::string name, std::string path) {
	m_Buffers.push_back(sf::SoundBuffer());
	m_Buffers.back().loadFromFile(path);

	m_Sounds.insert(std::pair<const std::string, sf::Sound>(name, sf::Sound()));

	int i = 0;
	for (auto& s : m_Sounds) {
		s.second.setBuffer(m_Buffers[i]);
		s.second.setVolume(0);
		i++;
	}
}

void MusicController::AddMusic(std::string name, std::string path) {
	m_Musics[name].openFromFile(path);
	m_Musics[name].setVolume(0);
}

sf::Sound& MusicController::GetSound(std::string name) {
	return m_Sounds[name];
}

sf::Music& MusicController::GetMusic(std::string name) {
	return m_Musics[name];
}

MusicController MusicController::s_instance;