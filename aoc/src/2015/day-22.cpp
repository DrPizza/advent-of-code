#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_22 : problem
{
	advent_22() noexcept : problem(22) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-22.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_22>() {
	advent_22 a;
	a.solve();
}
