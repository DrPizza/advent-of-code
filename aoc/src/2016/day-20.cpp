#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_20 : problem
{
	advent_20() noexcept : problem(20) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-20.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_20>() {
	advent_20 a;
	a.solve();
}