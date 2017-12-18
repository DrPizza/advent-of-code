#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_22 : problem
{
	advent_2016_22() noexcept : problem(2016, 22) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-22.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_22>() {
	advent_2016_22 a;
	a.solve();
}
