#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>

struct advent_2015_18 : problem
{
	advent_2015_18() noexcept : problem(2015, 18) {
	}

protected:
	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_18>() {
	advent_2015_18 a;
	a.solve();
}
