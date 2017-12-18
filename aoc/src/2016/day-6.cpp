#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_6 : problem
{
	advent_6() noexcept : problem(6) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-6.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_6>() {
	advent_6 a;
	a.solve();
}
