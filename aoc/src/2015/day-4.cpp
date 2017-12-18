#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_4 : problem
{
	advent_2015_4() noexcept : problem(2015, 4) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-4.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_4>() {
	advent_2015_4 a;
	a.solve();
}
