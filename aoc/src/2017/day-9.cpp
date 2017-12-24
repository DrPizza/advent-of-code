#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <unordered_map>

struct advent_2017_9 : problem
{
	advent_2017_9() noexcept : problem(2017, 9) {
	}

protected:
	std::string stream;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, stream);
	}

	std::size_t total_group_score = 0;
	std::size_t garbage_count = 0;

	enum struct state
	{
		normal,
		garbage,
		skip,
	};

	void precompute() noexcept override {
		std::size_t group_score = 0;
		state current_state = state::normal;
		for(const char ch : stream) {
			switch(current_state) {
			case state::normal:
				switch(ch) {
				case '{':
					++group_score;
					total_group_score += group_score;
					break;
				case '}':
					--group_score;
					break;
				case '<':
					current_state = state::garbage;
					break;
				}
				break;
			case state::garbage:
				switch(ch) {
				case '>':
					current_state = state::normal;
					break;
				case '!':
					current_state = state::skip;
					break;
				default:
					++garbage_count;
					break;
				}
				break;
			case state::skip:
				current_state = state::garbage;
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
