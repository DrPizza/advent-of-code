#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_18 : problem
{
	advent_18() noexcept : problem(18) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-18.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_18>() {
	advent_18 a;
	a.solve();
}
