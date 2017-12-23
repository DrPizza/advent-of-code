#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <valarray>

struct advent_2016_8 : problem
{
	advent_2016_8() noexcept : problem(2016, 8) {
	}

protected:
	std::vector<std::string> raw_instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_instructions.push_back(line);
		}
	}

	using screen_type = std::valarray<char>;
	constexpr static std::size_t height = 6;
	constexpr static std::size_t width = 50;

	void rect(screen_type& screen, std::size_t rows, std::size_t cols) {
		screen[std::gslice{ 0, {cols, rows}, {width, 1} }] = '#';
	}

	void rotate_row(screen_type& screen, std::size_t r, std::ptrdiff_t amount) {
		screen_type row = screen[std::slice{ width * r, width, 1 }];
		screen[std::slice{ width * r, width, 1 }] = row.cshift(-amount);
	}

	void rotate_column(screen_type& screen, std::size_t c, std::ptrdiff_t amount) {
		screen_type col = screen[std::slice{ c, height, width }];
		screen[std::slice{ c, height, width }] = col.cshift(-amount);
	}

	void print(const screen_type& screen) {
		for(std::size_t r = 0; r < height; ++r) {
			for(std::size_t c = 0; c < width; ++c) {
				std::cout << screen[(r * width) + c];
			}
			std::cout << std::endl;
		}
	}

	std::string part_1() override {
		screen_type screen('.', width * height);
		rect(screen, 3, 2);
		print(screen);
		rotate_column(screen, 1, 1);
		print(screen);
		rotate_row(screen, 0, 4);
		print(screen);
		rotate_column(screen, 1, -2);
		print(screen);
		return "";
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_8>() {
	advent_2016_8 a;
	a.solve();
}
