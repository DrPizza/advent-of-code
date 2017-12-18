#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_15 : problem
{
	advent_15() noexcept : problem(15) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-15.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_15>() {
	advent_15 a;
	a.solve();
}