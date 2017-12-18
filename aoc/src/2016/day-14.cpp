#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_14 : problem
{
	advent_14() noexcept : problem(14) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-14.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_14>() {
	advent_14 a;
	a.solve();
}
