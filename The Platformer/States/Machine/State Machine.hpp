#pragma once
#include "stdafx.h"
#include "Asset Manager.hpp"

#include "../Effects/Music controller/Music Controller.hpp"

class State;

class StateMachine {
public:
	StateMachine(sf::RenderWindow& window, const float& dt, const float& fdt);
	~StateMachine();

	void AddStateAtTop(State* state);
	void AddStateAtBottom(State* state);

	void FixedUpdate();
	void Update();
	void Render();

	void ClearAndPush(State* state);

	sf::RenderWindow& m_window;
	const float& m_fdt;
	const float& m_dt;

private:
	sf::Sprite		   m_frameSprite;
	sf::RenderTexture  m_frame;
	std::stack<State*> m_states;

	void ClearStates();
};

class State {
public:
	State(StateMachine& machine);
	virtual ~State();

	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void Render(sf::RenderTarget& target) = 0;

	const bool IsRunning() const;

	// Accessors
	sf::Vector2f GetMousePos() const;

protected:
	StateMachine& i_machine;

	sf::RenderWindow& i_window;
	const float& i_fdt;
	const float& i_dt;
	bool i_isRunning = true;
};