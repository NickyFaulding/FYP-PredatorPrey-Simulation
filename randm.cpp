#include <iostream>
#include <random>
#include <chrono>
#pragma once

static std::mt19937 random_number_engine(time(0));

static float RandomNumberGenerator(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(random_number_engine);
}