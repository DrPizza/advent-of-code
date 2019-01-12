#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <algorithm>

struct advent_2015_5 : problem
{
	advent_2015_5() noexcept : problem(2015, 5) {
	}

protected:
	std::vector<std::string> strings;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			strings.push_back(line);
		}
	}

	std::string part_1() override {
		const auto is_nice = [](const std::string& s) -> bool {
			if(s.find("ab") != -1
			|| s.find("cd") != -1
			|| s.find("pq") != -1
			|| s.find("xy") != -1) {
				return false;
			}
			const std::ptrdiff_t vowel_count = std::count_if(std::begin(s), std::end(s), [](char c) {
				return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
			});
			if(vowel_count < 3) {
				return false;
			}
			if(std::adjacent_find(std::begin(s), std::end(s)) == std::end(s)) {
				return false;
			}
			return true;
		};
		const std::ptrdiff_t nice = std::count_if(std::begin(strings), std::end(strings), is_nice);
		return std::to_string(nice);
	}

	std::string part_2() override {
		const auto is_nice = [](const std::string& s) -> bool {
			bool has_repeat = false;
			for(std::size_t i = 0; !has_repeat && i < s.size() - 3; ++i) {
				for(std::size_t j = i + 2; j < s.size() - 1; ++j) {
					if(s[i    ] == s[j    ]
					&& s[i + 1] == s[j + 1]) {
						has_repeat = true;
						break;
					}
				}
			}
			if(!has_repeat) {
				return false;
			}
			bool has_aba = false;
			for(std::size_t i = 0; i < s.size() - 2; ++i) {
				if(s[i] == s[i + 2]) {
					has_aba = true;
					break;
				}
			}
			if(!has_aba) {
				return false;
			}
			return true;
		};
		const std::ptrdiff_t nice = std::count_if(std::begin(strings), std::end(strings), is_nice);
		return std::to_string(nice);
	}
};

REGISTER_SOLVER(2015, 5);
