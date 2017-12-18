#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_9 : problem
{
	advent_9() noexcept : problem(9) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-9.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_9>() {
	advent_9 a;
	a.solve();
}