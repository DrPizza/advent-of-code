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

struct advent_19 : problem
{
	advent_19() noexcept : problem(19) {
	}

	void prepare_input() override {
		std::ifstream fin("day-19.txt");
		std::string line;
		std::getline(fin, line);
	}
};

template<>
void solve<advent_day::day_19>() {
	advent_19 a;
	a.solve();
}
