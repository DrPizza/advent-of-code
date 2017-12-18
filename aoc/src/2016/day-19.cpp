#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_19 : problem
{
	advent_19() noexcept : problem(19) {
	}

protected:
	void prepare_input() override {
		std::ifstream fin("input/2016/day-19.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_19>() {
	advent_19 a;
	a.solve();
}
