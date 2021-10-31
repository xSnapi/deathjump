#include "stdafx.h"
#include "Countdown.hpp"

#include "../Effects/Music controller/Music Controller.hpp"

#include "../Public Variables/Extern.hpp"

Countdown::Countdown(sf::Font* font) {
	this->font		= font;

	this->InitCounter();

	// tutaj przyszed³ du¿y pan sus amogus vent
	buffers[0].loadFromFile("res/sounds/3.wav");
	buffers[1].loadFromFile("res/sounds/2.wav");
	buffers[2].loadFromFile("res/sounds/1.wav");
	buffers[3].loadFromFile("res/sounds/go.wav");

	sounds["3"].setBuffer(buffers[0]);
	sounds["2"].setBuffer(buffers[1]);
	sounds["1"].setBuffer(buffers[2]);
	sounds["GO"].setBuffer(buffers[3]);
	//MusicController::Get().GetSound("3");
	sounds["3"].setVolume(Extern::sound_volume);
	sounds["3"].play();
}

Countdown::~Countdown() {

}

void Countdown::Update() {
	sf::Texture& tx = const_cast<sf::Texture&>(this->font->getTexture(140));
	tx.setSmooth(false);

	if (this->currentFrame > 4)
		return;
	if (this->clock.getElapsedTime().asSeconds() > 1.0f ) {
		this->currentFrame++;
		if (this->currentFrame < 4) {
			this->counter.setString(this->str[this->currentFrame]);
			this->SetCounterOrigin();
			this->counter.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, 35.f));
			switch (currentFrame)
			{
			case 1:
				sounds["2"].setVolume(Extern::sound_volume);
				sounds["2"].play();
				break;
			case 2:
				sounds["1"].setVolume(Extern::sound_volume);
				sounds["1"].play();
				break;
			case 3:
				sounds["GO"].setVolume(Extern::sound_volume);
				sounds["GO"].play();
				break;
			}	
		}
		this->clock.restart();
	}
}

unsigned Countdown::CurrentFrame() const {
	return this->currentFrame;
}


void Countdown::SetCounterOrigin() {
	this->counter.setOrigin(sf::Vector2f(this->counter.getGlobalBounds().width / 2.0f, this->counter.getGlobalBounds().height / 2.0f));
}

void Countdown::InitCounter() {
	this->counter.setFont(*this->font);

	this->counter.setCharacterSize(140);

	this->counter.setString("3");

	this->SetCounterOrigin();

	this->counter.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, 35.f));
}

void Countdown::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if(this->currentFrame < 4)
		target.draw(this->counter);
}
