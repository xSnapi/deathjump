#include "stdafx.h"
#include "State Machine.hpp"

#include "../Effects/Transition/Transition.hpp"

StateMachine::StateMachine
(
	sf::RenderWindow& window,
	const float& dt,
	const float& fdt
)
	:

	m_window(window),
	m_dt(dt),
	m_fdt(fdt)
{
	m_frame.create(WINDOW_WIDTH, WINDOW_HEIGHT);
}

StateMachine::~StateMachine() {
	ClearStates();
}

void StateMachine::AddStateAtTop(State* state) {
	m_states.push(state);
}

void StateMachine::AddStateAtBottom(State* state) {
	if (m_states.empty()) {
		m_states.push(state);
	}
	else {
		State* temp = m_states.top();
		m_states.pop();
		AddStateAtBottom(state);
		m_states.push(temp);
	}
}

void StateMachine::FixedUpdate() {
	if (!m_states.empty())
		m_states.top()->FixedUpdate();
}

void StateMachine::Update() {
	if (!m_states.empty()) {
		m_states.top()->Update();
		if (!m_states.top()->IsRunning()) {
			delete m_states.top();
			m_states.pop();
		}
	}
	else {
		m_window.close();
	}
}

void StateMachine::Render() {
	m_frame.clear();

	if (!m_states.empty())
		m_states.top()->Render(m_frame);
	//m_frame.draw(sf::RectangleShape(sf::Vector2f(1600, 900)));
	m_frame.display();

	m_frameSprite.setTexture(m_frame.getTexture());

	// two separate draw calls still sholudn't be this expensive because of rendertexture
	m_window.draw(m_frameSprite, &Transition::Get().GetShader());
	//m_window.draw(m_frameSprite, &ShaderLibrary::GetShader("vignette"));
}

void StateMachine::ClearAndPush(State* state) {
	ClearStates();
	m_states.push(state);
}

void StateMachine::ClearStates() {
	while (!m_states.empty()) {
		delete m_states.top();
		m_states.pop();
	}
}

State::State
(
	StateMachine& machine
)
	:

	i_machine(machine),
	i_window(machine.m_window),
	i_dt(machine.m_dt),
	i_fdt(machine.m_fdt)
{

}

State::~State() {

}

const bool State::IsRunning() const {
	return i_isRunning;
}

sf::Vector2f State::GetMousePos() const {
	return i_window.mapPixelToCoords(sf::Mouse::getPosition(i_window));
}
