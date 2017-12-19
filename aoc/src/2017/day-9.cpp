#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <unordered_map>

struct advent_2017_9 : problem
{
	advent_2017_9() noexcept : problem(2017, 9) {
	}

	std::string stream;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, stream);
	}

	std::size_t total_group_score = 0;
	std::size_t garbage_count = 0;

	enum state_t
	{
		normal,
		garbage,
		skip,
	};

	void precompute() noexcept override {
		std::size_t group_score = 0;
		state_t state = normal;
		for(const char ch : stream) {
			switch(state) {
			case normal:
				switch(ch) {
				case '{':
					++group_score;
					total_group_score += group_score;
					break;
				case '}':
					--group_score;
					break;
				case '<':
					state = garbage;
					break;
				}
				break;
			case garbage:
				switch(ch) {
				case '>':
					state = normal;
					break;
				case '!':
					state = skip;
					break;
				default:
					++garbage_count;
					break;
				}
				break;
			case skip:
				state = garbage;
				break;
			}
		}
	}

	std::string part_1() noexcept override {
		return std::to_string(total_group_score);
	}

	std::string part_2() noexcept override {
		return std::to_string(garbage_count);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_9>() {
	advent_2017_9 a;
	a.solve();
}
