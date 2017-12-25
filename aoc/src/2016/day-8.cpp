#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <valarray>
#include <regex>

struct advent_2016_8 : problem
{
	advent_2016_8() noexcept : problem(2016, 8) {
	}

protected:
	enum struct operation
	{
		rect,
		rot_row,
		rot_col
	};

	struct instruction
	{
		operation op;
		std::size_t a;
		std::size_t b;
	};

	std::vector<instruction> instructions;

	void prepare_input(std::ifstream& fin) override {
		std::regex rect_pattern(R"(rect ([[:digit:]]+)x([[:digit:]]+))");
		std::regex rot_row_pattern(R"(rotate row y=([[:digit:]]+) by ([[:digit:]]+))");
		std::regex rot_col_pattern(R"(rotate column x=([[:digit:]]+) by ([[:digit:]]+))");

		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			operation op = operation::rect;
			if(std::regex_search(line, m, rect_pattern)) {
				op = operation::rect;
			} else if(std::regex_search(line, m, rot_row_pattern)) {
				op = operation::rot_row;
			} else if(std::regex_search(line, m, rot_col_pattern)) {
				op = operation::rot_col;
			}
			instructions.push_back(instruction{ op, std::stoull(m[1].str()), std::stoull(m[2].str())});
		}
	}

	using screen_type = std::valarray<size_t>;
	constexpr static std::size_t height = 6;
	constexpr static std::size_t width = 50;
	constexpr static std::size_t letter_width = 5;
	screen_type screen = screen_type(width * height);

	static void rect(screen_type& screen, std::size_t rows, std::size_t cols) {
		screen[std::gslice{ 0, {cols, rows}, {width, 1} }] = 1;
	}

	static void rotate_row(screen_type& screen, std::size_t r, std::ptrdiff_t amount) {
		screen_type row = screen[std::slice{ width * r, width, 1 }];
		screen[std::slice{ width * r, width, 1 }] = row.cshift(-amount);
	}

	static void rotate_column(screen_type& screen, std::size_t c, std::ptrdiff_t amount) {
		screen_type col = screen[std::slice{ c, height, width }];
		screen[std::slice{ c, height, width }] = col.cshift(-amount);
	}

	static void print(const screen_type& screen) {
		for(std::size_t r = 0; r < height; ++r) {
			for(std::size_t c = 0; c < width; ++c) {
				const std::size_t val = screen[(r * width) + c];
				std::cout << ((val == 1) ? '#' : ' ');
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void execute(screen_type& s, instruction i) {
		switch(i.op) {
		case operation::rect:
			rect(s, i.a, i.b);
			break;
		case operation::rot_row:
			rotate_row(s, i.a, i.b);
			break;
		case operation::rot_col:
			rotate_column(s, i.a, i.b);
			break;
		}
	}

	std::string part_1() override {
		for(const instruction& i : instructions) {
			execute(screen, i);
		}
		const std::size_t lit = screen.sum();
		print(screen);
		return std::to_string(lit);;
	}

	std::string part_2() override {
		screen[std::gslice(0, { 10, height, letter_width }, { letter_width, width, 1 })] = 1;
		//screen_type letters = screen[std::gslice(0, {10, 5, 6}, { 30, 6, 1})];
		//for(std::size_t i = 0; i < 30; ++i) {
		//	//std::cout << letters[i];
		//	letters[i] = 1 - letters[i];
		//}
		print(screen);
		return "";
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_8>() {
	advent_2016_8 a;
	a.solve();
}
