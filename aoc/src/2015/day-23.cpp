#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_23 : problem
{
	advent_2015_23() noexcept : problem(2015, 23) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-23.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_23>() {
	advent_2015_23 a;
	a.solve();
}
