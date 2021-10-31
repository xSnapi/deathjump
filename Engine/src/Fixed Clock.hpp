#pragma once
#include <SFML/System/Clock.hpp>

class FixedClock {
public:
	sf::Time getElapsedTime()
	{
		if (this->clock_flag)
		{
			this->clock.restart();
			this->clock_flag = false;
		}
		return this->clock.getElapsedTime();
	}

	void restart()
	{
		this->clock.restart();
		this->clock_flag = true;
	}
private:
	sf::Clock clock;
	bool clock_flag = true;
};