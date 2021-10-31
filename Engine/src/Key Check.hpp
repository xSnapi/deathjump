#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <unordered_map>

class KeyCheck {
public:
	//KeyCheck(const KeyCheck&) = delete;

	//static KeyCheck& Get();

	static void Init();
	static void Update();

	// this functions below return true if buttons IS NOT PRESSED otherwies false
	const static bool Key(char key);
	const static bool Button(char button);
private:
	KeyCheck() {}
	~KeyCheck() {}

	static std::unordered_map<char, bool> s_Keys;
	static std::unordered_map<char, bool> s_Buttons;

	//static KeyCheck s_instance;
};
