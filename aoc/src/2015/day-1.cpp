#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_1 : problem
{
	advent_1() noexcept : problem(1) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-1.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_1>() {
	advent_1 a;
	a.solve();
}
