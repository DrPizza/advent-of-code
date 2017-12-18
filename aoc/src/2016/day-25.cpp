#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_25 : problem
{
	advent_25() noexcept : problem(25) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-25.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_25>() {
	advent_25 a;
	a.solve();
}
