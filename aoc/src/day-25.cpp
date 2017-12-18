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

struct advent_25 : problem
{
	advent_25() noexcept : problem(25) {
	}

	void prepare_input() override {
		std::ifstream fin("day-25.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_day::day_25>() {
	advent_25 a;
	a.solve();
}
