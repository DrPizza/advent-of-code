#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_19 : problem
{
	advent_2015_19() noexcept : problem(2015, 19) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2015/day-19.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_19>() {
	advent_2015_19 a;
	a.solve();
}
