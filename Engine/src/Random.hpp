#pragma once
#include <random>

class Random {
public:
	static int RandInt(int min, int max);
	static float RandFloat(float min, float max);

private:
	static std::random_device rgen;
};