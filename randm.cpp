#include <iostream>
#include <random>
#include <chrono>
#pragma once

static std::mt19937 random_number_engine(time(0));

static int RandomNumberGenerator(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(random_number_engine);
}