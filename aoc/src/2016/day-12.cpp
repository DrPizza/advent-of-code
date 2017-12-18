#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_12 : problem
{
	advent_2016_12() noexcept : problem(2016, 12) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-12.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_12>() {
	advent_2016_12 a;
	a.solve();
}
