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

struct advent_23 : problem
{
	advent_23() noexcept : problem(23) {
	}

	void prepare_input() override {
		std::ifstream fin("day-23.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_day::day_23>() {
	advent_23 a;
	a.solve();
}
