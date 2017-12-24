#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <functional>
#include <map>

struct advent_2015_3 : problem
{
	advent_2015_3() noexcept : problem(2015, 3) {
	}

protected:
	using coord = std::pair<std::ptrdiff_t, std::ptrdiff_t>;

	std::string directions;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, directions);
	}

	std::string part_1() override {
		std::map<coord, std::size_t> present_counts;
		coord position = { 0, 0 };
		present_counts[position] = 1;
		for(const char ch : directions) {
			switch(ch) {
			case '<':
				--position.first;
				++present_counts[position];
				break;
			case '^':
				++position.second;
				++present_counts[position];
				break;
			case '>':
				++position.first;
				++present_counts[position];
				break;
			case 'v':
				--position.second;
				++present_counts[position];
				break;
			}
		}
		return std::to_string(present_counts.size());
	}

	std::string part_2() override {
		std::map<coord, std::size_t> presents;
		coord positions[2] = { { 0, 0 }, {0, 0} };
		++presents[positions[0]];
		++presents[positions[1]];

		std::size_t idx = 0;
		for(const char ch : directions) {
			switch(ch) {
			case '<':
				--positions[idx].first;
				++presents[positions[idx]];
				break;
			case '^':
				++positions[idx].second;
				++presents[positions[idx]];
				break;
			case '>':
				++positions[idx].first;
				++presents[positions[idx]];
				break;
			case 'v':
				--positions[idx].second;
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
