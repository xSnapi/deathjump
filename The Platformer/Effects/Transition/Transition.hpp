#pragma once
#include "stdafx.h"

#include "Shader Library.hpp"

enum class Mode {
	Veil = 0,
	Unveil = 1
};

class Transition {
public:
	Transition(const Transition&) = delete;

	static Transition& Get();

	void Init();

	void Update(const float& dt);

	void Start(Mode mode);

	void SetFunction(std::function<void()> fun);

	const sf::Shader& GetShader() const;

private:
	Transition() {}
	~Transition() {}

	static Transition s_instance;

	sf::Shader* m_shader = nullptr;
	float m_updateTime   = 0.0075f;
	float m_elapsedTime  = 0.0f;
	float m_progress	 = 0.0f;

	Mode m_mode = Mode::Unveil;
	std::function<void()> m_fun = 0;

	void UpdateTransition(int multi);
};