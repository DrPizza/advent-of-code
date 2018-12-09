#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <valarray>
#include <regex>
#include <unordered_map>

struct advent_2016_8 : problem
{
	[[gsl::suppress(f.6)]]
	advent_2016_8() : problem(2016, 8) {
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
	constexpr static std::size_t total_pixels = height * width;
	constexpr static std::size_t total_letters = width / letter_width;
	constexpr static std::size_t pixels_per_letter = total_pixels / total_letters;

	screen_type screen = screen_type(total_pixels);

	static void rect(screen_type& screen, std::size_t rows, std::size_t cols) {
		screen[std::gslice{ 0, {cols, rows}, {width, 1} }] = 1;
	}

	static void rotate_row(screen_type& screen, std::size_t r, std::ptrdiff_t amount) {
		screen_type row = screen[std::slice{ width * r, width, 1 }];
		screen[std::slice{ width * r, width, 1 }] = row.cshift(gsl::narrow_cast<int>(-amount));
	}

	static void rotate_column(screen_type& screen, std::size_t c, std::ptrdiff_t amount) {
		screen_type col = screen[std::slice{ c, height, width }];
		screen[std::slice{ c, height, width }] = col.cshift(gsl::narrow_cast<int>(-amount));
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
			rotate_row(s, i.a, gsl::narrow_cast<std::ptrdiff_t>(i.b));
			break;
		case operation::rot_col:
			rotate_column(s, i.a, gsl::narrow_cast<std::ptrdiff_t>(i.b));
			break;
		}
	}

	std::string part_1() override {
		for(const instruction& i : instructions) {
			execute(screen, i);
		}
		const std::size_t lit = screen.sum();
		return std::to_string(lit);;
	}

	static const inline std::unordered_map<std::uint32_t, char> letter_codes = {{
		{0x19297A52, 'a'}, {0x392E4A5C, 'b'}, {0x1928424C, 'c'}, {0x39294A5C, 'd'}, {0x3D0E421E, 'e'},
		{0x3D0E4210, 'f'}, {0x19285A4E, 'g'}, {0x252F4A52, 'h'}, {0x1C42108E, 'i'}, {0x0C210A4C, 'j'},
		{0x254C5292, 'k'}, {0x2108421E, 'l'}, {0x23BAC631, 'm'}, {0x252D5A52, 'n'}, {0x19294A4C, 'o'},
		{0x39297210, 'p'}, {0x19295A4D, 'q'}, {0x39297292, 'r'}, {0x1D08305C, 's'}, {0x1C421084, 't'},
		{0x25294A4C, 'u'}, {0x2318A944, 'v'}, {0x231AD6AA, 'w'}, {0x22A22951, 'x'}, {0x23151084, 'y'},
		{0x3C22221E, 'z'}, {0x00000000, ' '}
	}};

	std::string part_2() override {
		screen_type letters = screen[std::gslice(0, { total_letters, height, letter_width }, { letter_width, width, 1 })];
		std::string result;
		for(std::size_t letter = 0; letter < total_letters; ++letter) {
			std::uint32_t letter_code = 0;
			for(std::size_t pixel = 0; pixel < pixels_per_letter; ++pixel) {
				letter_code = gsl::narrow_cast<std::uint32_t>(letter_code | (letters[(letter * pixels_per_letter) + pixel] << (30 - 1 - pixel)));
			}
			result += letter_codes.at(letter_code);
		}
		return result;
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_8>() {
	advent_2016_8 a;
	a.solve();
}
