#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_14 : problem
{
	advent_2016_14() noexcept : problem(2016, 14) {
	}

protected:
	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_14>() {
	advent_2016_14 a;
	a.solve();
}
