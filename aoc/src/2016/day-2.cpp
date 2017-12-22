#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_2 : problem
{
	advent_2016_2() noexcept : problem(2016, 2) {
	}

protected:
	enum struct direction : int8_t
	{
		up,
		right,
		down,
		left
	};

	std::vector<std::vector<direction>> sequences;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			std::vector<direction> sequence;
			for(const char c : line) {
				switch(c) {
				case 'U':
					sequence.push_back(direction::up);
					break;
				case 'R':
					sequence.push_back(direction::right);
					break;
				case 'D':
					sequence.push_back(direction::down);
					break;
				case 'L':
					sequence.push_back(direction::left);
					break;
				}
			}
			sequences.push_back(sequence);
		}
	}

	int8_t keypad_move(int8_t position, direction d) noexcept {
		const static int8_t destinations[4][10] = {
			{ 0, 1, 2, 3, 1, 2, 3, 4, 5, 6 },
			{ 0, 2, 3, 3, 5, 6, 6, 8, 9, 9 },
			{ 0, 4, 5, 6, 7, 8, 9, 7, 8, 9 },
			{ 0, 1, 1, 2, 4, 4, 5, 7, 7, 8 }
		};
		return destinations[gsl::narrow_cast<int8_t>(d)][position];
	}

	std::string part_1() override {
		int8_t position = 5;
		std::string code;
		for(const std::vector<direction>& sequence : sequences) {
			for(const direction d : sequence) {
				position = keypad_move(position, d);
			}
			code += std::to_string(position);
		}
		return code;
	}

	int8_t fancy_keypad_move(int8_t position, direction d) noexcept {
		const static int8_t destinations[4][14] = {
			{ 0, 1, 2, 1, 4, 5,   2,   3,   4, 9,   6,   7,   8, 0xb },
			{ 0, 1, 3, 4, 4, 6,   7,   8,   9, 9, 0xb, 0xc, 0xc, 0xd },
			{ 0, 3, 6, 7, 8, 5, 0xa, 0xb, 0xc, 9, 0xa, 0xd, 0xc, 0xd },
			{ 0, 1, 2, 2, 3, 5,   5,   6,   7, 8, 0xa, 0xa, 0xb, 0xd }
		};
		return destinations[gsl::narrow_cast<int8_t>(d)][position];
	}

	char to_char(int8_t position) {
		if(position < 10) {
			return position + '0';
		} else {
			return (position - 10) + 'A';
		}
	}

	std::string part_2() override {
		int8_t position = 5;
		std::string code;
		for(const std::vector<direction>& sequence : sequences) {
			for(const direction d : sequence) {
				position = fancy_keypad_move(position, d);
			}
			code += to_char(position);
		}
		return code;
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_2>() {
	advent_2016_2 a;
	a.solve();
}
