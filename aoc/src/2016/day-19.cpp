#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <cmath>

struct advent_2016_19 : problem
{
	advent_2016_19() noexcept : problem(2016, 19) {
	}

protected:
	std::uint64_t elf_count;

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
		elf_count = std::stoull(line);
	}

	std::string part_1() override {
		DWORD idx = 0;
		_BitScanReverse64(&idx, elf_count);
		std::uint64_t safe_position = elf_count;
		safe_position = gsl::narrow_cast<std::uint64_t>(safe_position & ((1 << idx) - 1));
		safe_position = gsl::narrow_cast<std::uint64_t>(safe_position << 1);
		safe_position = gsl::narrow_cast<std::uint64_t>(safe_position | 1);
		return std::to_string(safe_position);
	}

	std::string part_2() override {
		const double nearest_power_of_three = std::floor(std::log(elf_count - 1) / std::log(3.0));
		const double pow_three = std::pow(3.0, nearest_power_of_three);
		const double result = elf_count - pow_three + std::max(elf_count - (2.0 * pow_three), 0.0);
		const std::uint64_t safe_position = gsl::narrow_cast<std::uint64_t>(result);
		return std::to_string(safe_position);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_19>() {
	advent_2016_19 a;
	a.solve();
}
