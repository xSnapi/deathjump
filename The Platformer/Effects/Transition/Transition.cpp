#include "stdafx.h"
#include "Transition.hpp"

Transition& Transition::Get() {
	return s_instance;
}

void Transition::Init() {
	m_shader = &ShaderLibrary::Get().GetShader("transition");
}

void Transition::SetFunction(std::function<void()> fun) { m_fun = fun; }

void Transition::Update(const float& dt) {
	m_elapsedTime += dt;
	while (m_elapsedTime >= m_updateTime) {
		m_elapsedTime -= m_updateTime;
		UpdateTransition(((bool)m_mode ? 1 : -1));
	}
}

void Transition::Start(Mode mode) {
	m_mode = mode;
}

const sf::Shader& Transition::GetShader() const { return *m_shader; }


void Transition::UpdateTransition(int multi) {
	m_shader->setUniform("progress", m_progress);

	m_progress += 0.01f * multi;

	// do not ask
	if (multi > 0) {
		if (m_progress > 1.0f) {
			m_progress = 1.0f;
			if (m_fun) {
				m_fun();
				m_fun = 0;
			}
		}
	}
	else {
		if (m_progress < 0.0f) {
			m_progress = 0.0f;
			if (m_fun) {
				m_fun();
				m_fun = 0;
			}
		}
	}
}

Transition Transition::s_instance;

//float Transition::m_updateTime  = 0.0075f;
//float Transition::m_elapsedTime = 0.0f;
//float Transition::m_progress	= 0.0f;
//
//Mode Transition::m_mode = Mode::Unveil;
//
//sf::Shader* Transition::m_shader = nullptr;
//std::function<void()> Transition::m_fun = 0;