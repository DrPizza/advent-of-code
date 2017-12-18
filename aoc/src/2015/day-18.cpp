#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_18 : problem
{
	advent_2015_18() noexcept : problem(2015, 18) {
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
	advent_2015_18 a;
	a.solve();
}
