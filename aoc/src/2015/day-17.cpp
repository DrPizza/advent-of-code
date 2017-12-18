#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_17 : problem
{
	advent_17() noexcept : problem(17) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-17.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_17>() {
	advent_17 a;
	a.solve();
}
