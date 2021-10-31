#include "Timer.hpp"

void Timer::Update(const float& dt) {
	m_elapsedTime += dt;
	while (m_elapsedTime > m_updateTime) {
		m_fun();
		m_elapsedTime -= m_updateTime;
	}
}

void Timer::SetFunction(float time, std::function<void()> fun) {
	m_updateTime = time;
	m_fun = fun;
}

void Timer::SetUpdateTime(float time) {
	m_updateTime = time;
}

float Timer::GetUpdateTime() const {
	return m_updateTime;
}
