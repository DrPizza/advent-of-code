#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_10 : problem
{
	advent_10() noexcept : problem(10) {
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
	advent_10 a;
	a.solve();
}
