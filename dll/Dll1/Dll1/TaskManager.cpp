#include "pch.h"
#include <utility>
#include "TaskManager.h"
#include <random>

int test() {
	std::random_device dev;
	std::mt19937 rnd(dev());
	std::uniform_int_distribution<unsigned> dist(1, 100);
	unsigned random = dist(rnd);
	return random;
}