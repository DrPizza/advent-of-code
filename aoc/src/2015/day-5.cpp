#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_5 : problem
{
	advent_5() noexcept : problem(5) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-5.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_5>() {
	advent_5 a;
	a.solve();
}
