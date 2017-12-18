#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_12 : problem
{
	advent_12() noexcept : problem(12) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-12.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_12>() {
	advent_12 a;
	a.solve();
}
