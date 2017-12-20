#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>

struct advent_2017_23 : problem
{
	advent_2017_23() noexcept : problem(2017, 23) {
	}

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_23>() {
	advent_2017_23 a;
	a.solve();
}
