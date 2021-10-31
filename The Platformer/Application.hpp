#pragma once
#include "stdafx.h"

#include "Shader Library.hpp"
#include "Key Check.hpp"

#include "States/Machine/State Machine.hpp"
#include "States/State/Menu/Menu.hpp"
#include "States/Save.hpp"

#include "Effects/Transition/Transition.hpp"
#include "Effects/Music Controller/Music Controller.hpp"
#include "Effects/Camera/Camera.hpp"

class Application {
public:
	Application(const float& dt, const float& fdt);
	~Application();

	void Update();
	void FixedUpdate();

	void Render();

	bool IsRunning() const;
private:
	sf::RenderWindow m_window;
	sf::Event m_event;

	StateMachine* m_machine;

	const float& m_fdt;
	const float& m_dt;
	
	sf::Vector2i m_windowOffset;
	bool m_windowGrabbed = false;

	void MoveWindow();

	void LoadMusicsAndSounds();

	void SetShaderUniforms();
	void LoadShaders();

	void InitStates();

	void HandleEvents();
	void InitWindow();
};
