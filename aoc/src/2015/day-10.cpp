#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_10 : problem
{
	advent_2015_10() noexcept : problem(2015, 10) {
	}

protected:
	std::string input;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, input);
	}

	std::string look_and_say(const std::string& in) {
		std::string result;
		result.reserve(gsl::narrow_cast<std::size_t>(in.size() * 1.40));
		char last_seen = in[0];
		std::size_t count = 0;
		for(const char ch : in) {
			if(ch != last_seen) {
				result += gsl::narrow_cast<char>(count + '0');
				result += last_seen;
				last_seen = ch;
				count = 1;
			} else {
				++count;
			}
		}
		result += gsl::narrow_cast<char>(count + '0');
		result += last_seen;
		return result;
	}

	std::string part_1() override {
		std::string result = input;
		for(std::size_t i = 0; i < 40; ++i) {
			result = std::move(look_and_say(result));
		}
		return std::to_string(result.size());
	}

	std::string part_2() override {
		std::string result = input;
		for(std::size_t i = 0; i < 50; ++i) {
			result = std::move(look_and_say(result));
		}
		return std::to_string(result.size());
	}
};

REGISTER_SOLVER(2015, 10);
