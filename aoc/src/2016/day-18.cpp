#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_18 : problem
{
	advent_2016_18() noexcept : problem(2016, 18) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-18.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_18>() {
	advent_2016_18 a;
	a.solve();
}
