#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_10 : problem
{
	advent_2015_10() noexcept : problem(2015, 10) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-10.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_10>() {
	advent_2015_10 a;
	a.solve();
}
