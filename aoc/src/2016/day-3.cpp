#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_3 : problem
{
	advent_2016_3() noexcept : problem(2016, 3) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-3.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_3>() {
	advent_2016_3 a;
	a.solve();
}
