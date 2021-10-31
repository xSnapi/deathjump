#pragma once
#include "stdafx.h"

#include "Fixed Clock.hpp"

class Countdown : public sf::Drawable {
public:
	Countdown(sf::Font* font);
	~Countdown();

	void Update();

	unsigned CurrentFrame() const;
private:
	sf::Font* font = 0;

	sf::Text counter;

	FixedClock clock;

	unsigned currentFrame = 0;

	sf::SoundBuffer buffers[4];
	std::unordered_map<std::string, sf::Sound> sounds;

	std::string str[4] =
	{
		"3",
		"2",
		"1",
		"GO!"
	};

	bool _IsRunning = true;

	void SetCounterOrigin();

	void InitCounter();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};