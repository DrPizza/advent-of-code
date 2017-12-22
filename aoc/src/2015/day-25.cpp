#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <regex>

struct advent_2015_25 : problem
{
	advent_2015_25() noexcept : problem(2015, 25) {
	}

protected:
	std::size_t row = 0;
	std::size_t col = 0;

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
		std::regex pattern(R"(To continue, please consult the code grid in the manual.  Enter the code at row ([[:digit:]]+), column ([[:digit:]]+).)");
		std::smatch m;
		std::regex_search(line, m, pattern);
		row = std::stoull(m[1]);
		col = std::stoull(m[2]);
	}

	std::size_t seed = 20'151'125;
	std::size_t current_code = seed;

	std::size_t generate_code() noexcept {
		current_code = current_code * 252'533;
		current_code = current_code % 33'554'393;
		return current_code;
	}

	std::size_t diagonalize(std::size_t r, std::size_t c) noexcept {
		const auto triangle = [](const std::size_t x) {
			return (x * (x + 1)) / 2;
		};

		return triangle(r + c - 1) - (r - 1);
	}

	std::string part_1() override {
		const std::size_t n = diagonalize(row, col);
		std::size_t code = 0;
		for(std::size_t i = 1; i < n; ++i) {
			code = generate_code();
		}
		return std::to_string(code);
	}

	std::string part_2() override {
		return "**************************************************";
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_25>() {
	advent_2015_25 a;
	a.solve();
}
