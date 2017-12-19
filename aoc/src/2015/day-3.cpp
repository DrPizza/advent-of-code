#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <functional>
#include <unordered_map>

namespace {
	struct coord
	{
		std::ptrdiff_t x, y;
	};

	static bool operator==(const coord& lhs, const coord& rhs) noexcept {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	struct coord_hash
	{
		std::size_t operator()(const coord& c) const noexcept {
			return std::hash<std::ptrdiff_t>{}(c.x) ^ std::hash<std::ptrdiff_t>{}(c.y);
		}
	};
}

struct advent_2015_3 : problem
{
	advent_2015_3() noexcept : problem(2015, 3) {
	}

protected:
	std::string directions;

	void prepare_input() override {
		std::ifstream fin("input/2015/day-3.txt");
		std::getline(fin, directions);
	}

	std::string part_1() override {
		std::unordered_map<coord, std::size_t, coord_hash> present_counts;
		coord position = { 0, 0 };
		present_counts[position] = 1;
		for(const char ch : directions) {
			switch(ch) {
			case '<':
				--position.x;
				++present_counts[position];
				break;
			case '^':
				++position.y;
				++present_counts[position];
				break;
			case '>':
				++position.x;
				++present_counts[position];
				break;
			case 'v':
				--position.y;
				++present_counts[position];
				break;
			}
		}
		return std::to_string(present_counts.size());
	}

	std::string part_2() override {
		std::unordered_map<coord, std::size_t, coord_hash> presents;
		coord positions[2] = { { 0, 0 }, {0, 0} };
		++presents[positions[0]];
		++presents[positions[1]];

		std::size_t idx = 0;
		for(const char ch : directions) {
			switch(ch) {
			case '<':
				--positions[idx].x;
				++presents[positions[idx]];
				break;
			case '^':
				++positions[idx].y;
				++presents[positions[idx]];
				break;
			case '>':
				++positions[idx].x;
				++presents[positions[idx]];
				break;
			case 'v':
				--positions[idx].y;
				++presents[positions[idx]];
				break;
			}
			idx = 1 - idx;
		}
		return std::to_string(presents.size());
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_3>() {
	advent_2015_3 a;
	a.solve();
}
