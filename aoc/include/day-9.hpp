#pragma once

#include "problem.hpp"

#include <fstream>
#include <unordered_map>

struct advent_9 : problem {
	advent_9() noexcept : problem(8) {
	}

	std::string stream;

	void prepare_input() override {
		std::ifstream fin("day-9.txt");
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
