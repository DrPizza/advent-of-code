#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_18 : problem
{
	advent_2016_18() noexcept : problem(2016, 18) {
	}

protected:
	std::string first_row;
	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, first_row);
	}

	std::string next_row(const std::string& s) {
		const std::string fake_row = "." + s + ".";
		std::string new_row;
		for(std::size_t i = 1; i < fake_row.size() - 1; ++i) {
			const bool trap = fake_row[i - 1] != fake_row[i + 1];
			new_row.push_back(trap ? '^' : '.');
		}
		return new_row;
	}

	std::string part_1() override {
		std::size_t safe_count = 0;
		std::string row = first_row;
		for(std::size_t i = 0; i < 40; ++i) {
			safe_count += std::count(std::begin(row), std::end(row), '.');
			row = next_row(row);
		}
		return std::to_string(safe_count);
	}

	std::string part_2() override {
		std::size_t safe_count = 0;
		std::string row = first_row;
		for(std::size_t i = 0; i < 400'000; ++i) {
			safe_count += std::count(std::begin(row), std::end(row), '.');
			row = next_row(row);
		}
		return std::to_string(safe_count);
	}

};

template<>
void solve<advent_year::year_2016, advent_day::day_18>() {
	advent_2016_18 a;
	a.solve();
}
