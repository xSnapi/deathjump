#pragma once
#include <SFML/System/Clock.hpp>
#include <functional>

class Timer {
public:
	void Update(const float& dt);

	void SetFunction(float time, std::function<void()> fun);
	void SetUpdateTime(float time);

	float GetUpdateTime() const;
private:
	float m_elapsedTime = 0.0f;
	float m_updateTime  = 9999999.0f;

	std::function<void()> m_fun;
};