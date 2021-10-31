#include "stdafx.h"
#include "Application.hpp"

int main() {
	sf::Clock deltaClock;
	sf::Clock fixedDeltaClock;

	float deltaTime			  = 0.0f;
	float fixedDeltaTime	  = 0.0f;
	double physicsElapsedTime = 0.0;

	Application application(deltaTime, fixedDeltaTime);

	deltaClock.restart();
	fixedDeltaClock.restart();

	while (application.IsRunning()) {
		/* Calculating time for fixed update to run */
		physicsElapsedTime += deltaTime;
		while (physicsElapsedTime > PHYSICS_STEP) {
			fixedDeltaTime = fixedDeltaClock.restart().asSeconds();
			if (fixedDeltaTime > PHYSICS_STEP * 5.0f)
				fixedDeltaTime = PHYSICS_STEP;

			application.FixedUpdate();
			physicsElapsedTime -= PHYSICS_STEP;
		}

		application.Update();
		application.Render();

		deltaTime = deltaClock.restart().asSeconds();
	}

	return EXIT_SUCCESS;
}