#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_7 : problem
{
	advent_2016_7() noexcept : problem(2016, 7) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-7.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_7>() {
	advent_2016_7 a;
	a.solve();
}
