#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_23 : problem
{
	advent_2016_23() noexcept : problem(2016, 23) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-23.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_23>() {
	advent_2016_23 a;
	a.solve();
}
