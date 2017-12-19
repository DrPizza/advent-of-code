#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_11 : problem
{
	advent_2015_11() noexcept : problem(2015, 11) {
	}

protected:
	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_11>() {
	advent_2015_11 a;
	a.solve();
}
