#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_16 : problem
{
	advent_2015_16() noexcept : problem(2015, 16) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-16.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_16>() {
	advent_2015_16 a;
	a.solve();
}
