#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <iomanip>
#include <regex>
#include <utility>
#include <map>
#include <numeric>
#include <chrono>
#include <boost/algorithm/string.hpp>

struct advent_21 : problem
{
	advent_21() noexcept : problem(21) {
	}

	void prepare_input() override {
		std::ifstream fin("input/2017/day-21.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_21>() {
	advent_21 a;
	a.solve();
}
