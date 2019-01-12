#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

struct advent_2018_11 : problem
{
	advent_2018_11() noexcept : problem(2018, 11) {
	}

protected:
	std::intmax_t serial_number = {};

	using grid_type = std::vector<std::vector<std::intmax_t> >;
	grid_type grid;

	void prepare_input(std::ifstream& fin) override {
		grid.resize(301, std::vector<std::intmax_t>(301, 0));

		std::string line;
		std::getline(fin, line);
		serial_number = std::stoll(line);

		for(std::size_t c = 1; c <= 300; ++c) {
			for(std::size_t r = 1; r <= 300; ++r) {
				const std::intmax_t rack_id = c + 10;
				std::intmax_t power_level = rack_id * r;
				power_level += serial_number;
				power_level *= rack_id;
				power_level = (power_level % 1'000) / 100;
				power_level -= 5;
				grid[r][c] = power_level + grid[r - 1][c] + grid[r][c - 1] - grid[r - 1][c - 1];
			}
		}
	}

	std::ptrdiff_t get_power(std::size_t r, std::size_t c, std::size_t s) noexcept {
		return grid[r][c] - grid[r - s][c] - grid[r][c - s] + grid[r - s][c - s];
	}

	std::string part_1() override {
		std::intmax_t max_power = std::numeric_limits<std::intmax_t>::min();
		std::size_t x = 0;
		std::size_t y = 0;
		for(std::size_t r = 3; r <= 300; ++r) {
			for(std::size_t c = 3; c <= 300; ++c) {
				const std::intmax_t total_power = get_power(r, c, 3);
				if(total_power > max_power) {
					max_power = total_power;
					x = c;
					y = r;
				}
			}
		}
		return std::to_string(x - 2) + "," + std::to_string(y - 2);
	}

	std::string part_2() override {
		std::intmax_t max_power = std::numeric_limits<std::intmax_t>::min();
		std::size_t x = 0;
		std::size_t y = 0;
		std::size_t size = 0;
		for(std::size_t s = 1; s <= 300; ++s) {
			for(std::size_t r = s; r <= 300; ++r) {
				for(std::size_t c = s; c <= 300; ++c) {
					const std::intmax_t total_power = get_power(r, c, s);
					if(total_power > max_power) {
						max_power = total_power;
						x = c;
						y = r;
						size = s;
					}
				}
			}
		}
		return std::to_string(x - size + 1) + "," + std::to_string(y - size + 1) + "," + std::to_string(size);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_11>() {
	advent_2018_11 a;
	a.solve();
}
