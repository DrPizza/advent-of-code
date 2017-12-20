#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>

struct advent_2015_17 : problem
{
	advent_2015_17() noexcept : problem(2015, 17) {
	}

protected:
	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_17>() {
	advent_2015_17 a;
	a.solve();
}
