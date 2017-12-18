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

struct advent_2017_20 : problem
{
	advent_2017_20() noexcept : problem(2017, 20) {
	}

	void prepare_input() override {
		std::ifstream fin("input/2017/day-20.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_20>() {
	advent_2017_20 a;
	a.solve();
}
