#pragma once

#include <functional>
#include <random>

using ProbabilityGenerator = std::function<double()>;

extern std::random_device rd;
extern std::mt19937 rng;

double default_probability_generator();

extern ProbabilityGenerator probability_generator;
