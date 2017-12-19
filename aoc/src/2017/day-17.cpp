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

struct advent_2017_17 : problem
{
	advent_2017_17() noexcept : problem(2017, 17) {
	}

	std::size_t steps = 0;

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
		steps = std::stoull(line);
	}

	std::string part_1() override {
		std::vector<std::size_t> buffer;
		buffer.push_back(0);
		std::size_t current_position = 0;
		for(std::size_t i = 1; i <= 2017; ++i) {
			current_position = ((current_position + steps) % i) + 1ui64;
			buffer.insert(std::begin(buffer) + gsl::narrow<std::ptrdiff_t>(current_position), i);
		}
		const auto position = std::find(std::begin(buffer), std::end(buffer), 2017ui64) + 1;
		return std::to_string((position != std::end(buffer)) ? *position : 0ui64);
	}

	std::string part_2() override {
		std::size_t current_position = 0;
		std::size_t after_zero = 0;
		for(std::size_t i = 1; i <= 50'000'000; ++i) {
			current_position = ((current_position + steps) % i) + 1ui64;
			if(current_position == 1) {
				after_zero = i;
			}
		}
		return std::to_string(after_zero);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_17>() {
	advent_2017_17 a;
	a.solve();
}
