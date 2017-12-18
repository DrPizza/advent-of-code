#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_8 : problem
{
	advent_8() noexcept : problem(8) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-8.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_8>() {
	advent_8 a;
	a.solve();
}
