#pragma once
#pragma once

#include "problem.hpp"

#include <fstream>
#include <unordered_map>

struct advent_9 : problem {
	advent_9() noexcept : problem(8) {
	}

	std::string garbage;

	void prepare_input() override {
		std::ifstream fin("day-9.txt");
		std::getline(fin, garbage);
	}

	std::size_t total_group_score = 0;
	std::size_t garbage_count = 0;

	void precompute() {
		std::size_t group_score = 0;
		bool in_garbage = false;
		bool skip_next = false;
		for(char ch : garbage) {
			if(skip_next) {
				skip_next = false;
				continue;
			}
			if(in_garbage) {
				switch(ch) {
				case '>':
					in_garbage = false;
					break;
				case '!':
					skip_next = true;
					break;
				default:
					++garbage_count;
					break;
				}
			} else {
				switch(ch) {
				case '{':
					++group_score;
					total_group_score += group_score;
					break;
				case '}':
					--group_score;
					break;
				case '<':
					in_garbage = true;
					break;
				}
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

