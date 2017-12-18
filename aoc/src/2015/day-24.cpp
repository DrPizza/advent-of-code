#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_24 : problem
{
	advent_24() noexcept : problem(24) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-24.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_24>() {
	advent_24 a;
	a.solve();
}
