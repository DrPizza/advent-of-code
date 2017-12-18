#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2 : problem
{
	advent_2() noexcept : problem(2) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-2.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_2>() {
	advent_2 a;
	a.solve();
}
