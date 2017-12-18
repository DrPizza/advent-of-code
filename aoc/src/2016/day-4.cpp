#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_4 : problem
{
	advent_4() noexcept : problem(4) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-4.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_4>() {
	advent_4 a;
	a.solve();
}
