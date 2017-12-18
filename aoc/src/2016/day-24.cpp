#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_24 : problem
{
	advent_2016_24() noexcept : problem(2016, 24) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-24.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_24>() {
	advent_2016_24 a;
	a.solve();
}
