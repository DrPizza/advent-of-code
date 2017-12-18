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

struct advent_22 : problem
{
	advent_22() noexcept : problem(22) {
	}

	void prepare_input() override {
		std::ifstream fin("day-22.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_day::day_22>() {
	advent_22 a;
	a.solve();
}
