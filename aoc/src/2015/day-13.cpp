#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_13 : problem
{
	advent_13() noexcept : problem(13) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-13.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_13>() {
	advent_13 a;
	a.solve();
}