#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_11 : problem
{
	advent_2016_11() noexcept : problem(2016, 11) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-11.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_11>() {
	advent_2016_11 a;
	a.solve();
}
