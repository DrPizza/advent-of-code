#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_5 : problem
{
	advent_2016_5() noexcept : problem(2016, 5) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-5.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_5>() {
	advent_2016_5 a;
	a.solve();
}
