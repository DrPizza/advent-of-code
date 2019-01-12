#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_11 : problem
{
	advent_2015_11() noexcept : problem(2015, 11) {
	}

protected:
	std::string password;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, password);
	}

	bool is_valid_password(const std::string& s) {
		if(s.find_first_of("iol") != -1) {
			return false;
		}
		bool has_ascending = false;
		std::size_t pairs = s[0] == s[1] ? 1ui64 : 0ui64;
		char last_pair = pairs > 0 ? s[0] : 0;
		for(std::size_t i = 2; i < s.size(); ++i) {
			if(s[i - 2] + 2 == s[i]
			&& s[i - 1] + 1 == s[i]) {
				has_ascending = true;
			}
			if(s[i - 1] == s[i] && s[i] != last_pair) {
				++pairs;
				last_pair = s[i];
			}
		}
		return has_ascending && pairs > 1;
	}

	char next_letter(char c) noexcept {
		return c == 'z' ? 'a' : c + 1 + (c == 'h' || c == 'n' || c == 'k');
	}

	void increment_password(std::string& s) {
		for(std::size_t i = 0; i < s.size(); ++i) {
			char c = next_letter(s[s.size() - i - 1]);
			s[s.size() - i - 1] = c;
			if(c != 'a') {
				break;
			}
		}
	}

	std::string part_1() override {
		do {
			increment_password(password);
		} while(!is_valid_password(password));

		return password;
	}

	std::string part_2() override {
		return part_1();
	}
};

REGISTER_SOLVER(2015, 11);
