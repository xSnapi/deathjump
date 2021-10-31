#include "Random.hpp"

int Random::RandInt(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(rgen);
}

float Random::RandFloat(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(rgen);
}

std::random_device Random::rgen;