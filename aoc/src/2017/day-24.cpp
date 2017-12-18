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

struct advent_24 : problem
{
	advent_24() noexcept : problem(24) {
	}

	void prepare_input() override {
		std::ifstream fin("input/2017/day-24.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_24>() {
	advent_24 a;
	a.solve();
}
