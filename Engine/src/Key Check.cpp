#include "Key Check.hpp"

//KeyCheck& KeyCheck::Get() {
//	return s_instance;
//}

void KeyCheck::Init() {
	for (int i = 0; i < 26; i++)
		s_Keys[i + 65] = false;

	s_Keys[' '] = false;

	s_Buttons['L'] = false;
	s_Buttons['R'] = false;
	s_Buttons['M'] = false;
}

void KeyCheck::Update() {
	// if you are asking yourself why i did't do just m_Keys[i + 65] = sf::Keyboard::isKeyPressed 
	// it's because everything was getting fucked up

	for (int i = 0; i < 26; i++)
		if (!sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i))
			s_Keys[i + 65] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		s_Keys[' '] = false;

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		s_Keys['/'] = false; // <- escape key B))


	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		s_Buttons['L'] = false;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
		s_Buttons['R'] = false;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		s_Buttons['M'] = false;
}

const bool KeyCheck::Button(char button) { 
	bool pressed = s_Buttons[button];

	if (!pressed)
		s_Buttons[button] = true;

	return !pressed;
}

const bool KeyCheck::Key(char key) { 
	bool pressed = s_Keys[key];

	if (!pressed)
		s_Keys[key] = true;

	return !pressed;
}

std::unordered_map<char, bool> KeyCheck::s_Keys = {};
std::unordered_map<char, bool> KeyCheck::s_Buttons = {};

//KeyCheck KeyCheck::s_instance;