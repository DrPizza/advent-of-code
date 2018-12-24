#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <unordered_map>

struct advent_2018_12 : problem
{
	advent_2018_12() : problem(2018, 12) {
	}

protected:
	std::string initial_state;
	std::array<char, 32> rules;

	static std::size_t pattern_to_index(const std::string& s) {
		std::size_t idx = 0;
		for(std::size_t i = 0ui64; i < 5ui64; ++i) {
			idx |= (s[i] == '#' ? 1ui64 : 0ui64) << i;
		}
		return idx;
	}

	void prepare_input(std::ifstream& fin) override {
		std::regex initial_pattern(R"(initial state: (.*))");
		std::string first_line;
		std::getline(fin, first_line);
		std::smatch m;
		std::regex_match(first_line, m, initial_pattern);
		initial_state = m[1];

		rules.fill('.');
		std::regex rule_pattern(R"((.....) => (.))");
		for(std::string line; std::getline(fin, line);) {
			if(std::regex_match(line, m, rule_pattern)) {
				rules[pattern_to_index(m[1].str())] = m[2].str()[0];
			}
		}
	}

	static std::ptrdiff_t get_value(std::ptrdiff_t zero_index, const std::string& state) {
		std::ptrdiff_t value = 0i64;
		for(std::size_t i = 0; i < state.size(); ++i) {
			if(state[i] == '#') {
				value += gsl::narrow_cast<std::ptrdiff_t>(i) - zero_index;
			}
		}
		return value;
	}

	std::string part_1() override {
		std::string state = initial_state;
		std::string rule = ".....";
		std::ptrdiff_t zero_index = 0;
		for(std::size_t generation = 0; generation < 20; ++generation) {
			state = "...." + state + "....";
			zero_index += 4;
			std::string next_state(state.size(), '.');
			for(std::size_t i = 2; i < state.size() - 2; ++i) {
				rule[0] = state[i - 2];
				rule[1] = state[i - 1];
				rule[2] = state[i    ];
				rule[3] = state[i + 1];
				rule[4] = state[i + 2];
				
				next_state[i] = rules[pattern_to_index(rule)];
			}
			state.swap(next_state);
		}

		return std::to_string(get_value(zero_index, state));
	}

	std::string part_2() override {
		std::string state = initial_state;
		std::string rule = ".....";
		std::ptrdiff_t zero_index = 0;
		std::ptrdiff_t previous_value = 0;
		std::ptrdiff_t previous_delta_1 = 0;
		std::ptrdiff_t previous_delta_2 = 0;
		std::ptrdiff_t running_total = 0;
		std::ptrdiff_t generation = 0;
		for(; ; ++generation) {
			state = "...." + state + "....";
			zero_index += 4;
			std::string next_state(state.size(), '.');
			for(std::size_t i = 2; i < state.size() - 2; ++i) {
				rule[0] = state[i - 2];
				rule[1] = state[i - 1];
				rule[2] = state[i    ];
				rule[3] = state[i + 1];
				rule[4] = state[i + 2];

				next_state[i] = rules[pattern_to_index(rule)];
			}
			state.swap(next_state);

			const std::ptrdiff_t value = get_value(zero_index, state);
			const std::ptrdiff_t delta = value - previous_value;

			if(delta == previous_delta_1 && previous_delta_1 == previous_delta_2) {
				running_total -= delta;
				running_total -= delta;
				generation -= 2;
				break;
			}
			running_total += delta;
			previous_value = value;
			previous_delta_2 = previous_delta_1;
			previous_delta_1 = delta;
		}
		const std::ptrdiff_t computed_score = ((50'000'000'000 - generation) * previous_delta_1) + running_total;
		return std::to_string(computed_score);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_12>() {
	advent_2018_12 a;
	a.solve();
}
