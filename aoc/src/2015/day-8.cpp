#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_8 : problem
{
	advent_2015_8() noexcept : problem(2015, 8) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-8.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_8>() {
	advent_2015_8 a;
	a.solve();
}
