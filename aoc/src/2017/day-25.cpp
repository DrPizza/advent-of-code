#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <array>

struct advent_2017_25 : problem
{
	advent_2017_25() noexcept : problem(2017, 25) {
	}

protected:
	enum struct direction
	{
		left, right
	};

	struct transition
	{
		bool new_value;
		direction new_direction;
		char new_state;
	};

	using state = std::array<transition, 2>;

	std::unordered_map<char, state> machine;
	char initial_state = 'A';
	std::size_t steps = 0;
	std::map<std::ptrdiff_t, bool> tape;

	void prepare_input(std::ifstream& fin) override {
		std::string config;
		for(std::string line; std::getline(fin, line);) {
			config += line;
			config += "\n";
		}
		std::regex header_pattern(
R"(Begin in state ([[:upper:]]).
Perform a diagnostic checksum after ([[:digit:]]+) steps.)"
		);
		std::regex rule_pattern(
R"(In state ([[:upper:]]):
  If the current value is 0:
    - Write the value ([[:digit:]]).
    - Move one slot to the (left|right).
    - Continue with state ([[:upper:]]).
  If the current value is 1:
    - Write the value ([[:digit:]]).
    - Move one slot to the (left|right).
    - Continue with state ([[:upper:]]).
)"
		);
		std::smatch m;
		std::regex_search(config, m, header_pattern);
		initial_state = m[1].str()[0];
		steps = std::stoull(m[2]);

		while(std::regex_search(config, m, rule_pattern)) {
			state s;
			s[0].new_value = m[2].str() == "1" ? true : false;
			s[0].new_direction = m[3].str() == "left" ? direction::left : direction::right;
			s[0].new_state = m[4].str()[0];
			s[1].new_value = m[5].str() == "1" ? true : false;
			s[1].new_direction = m[6].str() == "left" ? direction::left : direction::right;
			s[1].new_state = m[7].str()[0];

			machine[m[1].str()[0]] = s;
			config = m.suffix();
		}
	}

	std::string part_1() override {
		char st = initial_state;
		std::ptrdiff_t tape_position = 0;
		for(std::size_t i = 0; i < steps; ++i) {
			transition& t = machine[st][tape[tape_position]];
			tape[tape_position] = t.new_value;
			tape_position += t.new_direction == direction::left ? -1 : 1;
			st = t.new_state;
		}
		std::size_t ones = 0;
		for(const auto& x : tape) {
			ones += x.second ? 1 : 0;
		}
		return std::to_string(ones);
	}

	std::string part_2() override {
		return "*";
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_25>() {
	advent_2017_25 a;
	a.solve();
}
