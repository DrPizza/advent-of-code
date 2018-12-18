#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

struct advent_2018_2 : problem
{
	advent_2018_2() noexcept : problem(2018, 2) {
	}

protected:
	void prepare_input(std::ifstream& fin) override {
	}

	std::string part_1() override {
		return "";
	}

	std::string part_2() override {
		return "";
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_2>() {
	advent_2018_2 a;
	a.solve();
}
