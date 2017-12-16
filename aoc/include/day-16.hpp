#pragma once

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <regex>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <boost/algorithm/string.hpp>

struct advent_16 : problem
{
	advent_16() noexcept : problem(16) {
	}

	std::string dancers;

	void do_spin(std::ptrdiff_t amount) {
		std::rotate(std::rbegin(dancers), std::rbegin(dancers) + amount, std::rend(dancers));
	}

	void do_exchange(std::size_t a, std::size_t b) {
		std::swap(dancers[a], dancers[b]);
	}

	void do_partner(char a, char b) {
		std::iter_swap(std::find(std::begin(dancers), std::end(dancers), a),
		               std::find(std::begin(dancers), std::end(dancers), b));
	}

	std::string raw_commands;

	void prepare_input() override {
		std::ifstream fin("day-16.txt");
		std::string line;
		std::getline(fin, raw_commands);
	}

	using dance_move = std::function<void(void)>;

	std::vector<dance_move> moves;

	void precompute() override {
		dancers.resize(16);
		std::iota(std::begin(dancers), std::end(dancers), 'a');

		std::regex pattern(R"((s([[:digit:]]+))|(x([[:digit:]]+)/([[:digit:]]+))|(p([[:lower:]])/([[:lower:]])),?)");
		std::sregex_iterator it(std::begin(raw_commands), std::end(raw_commands), pattern);
		std::sregex_iterator end;
		for(; it != end; ++it) {
			const std::smatch m = *it;
			if(m[1].matched) {
				std::ptrdiff_t amount = std::stoll(m[2].str());
				moves.push_back([=]() {
					do_spin(amount);
				});
			} else if(m[3].matched) {
				std::size_t a = std::stoull(m[4].str());
				std::size_t b = std::stoull(m[5].str());
				moves.push_back([=]() {
					do_exchange(a, b);
				});
			} else if(m[6].matched) {
				char a = m[7].str()[0];
				char b = m[8].str()[0];
				moves.push_back([=]() {
					do_partner(a, b);
				});
			}
		}
	}

	std::string part_1() override {
		for(const auto& m : moves) {
			m();
		}
		return dancers;
	}

	std::size_t find_cycle_length() {
		const std::string original = "abcdefghijklmnop";
		std::iota(std::begin(dancers), std::end(dancers), gsl::narrow<uint8_t>('a'));
		std::size_t cycle_length = 0;
		do {
			for(const auto& m : moves) {
				m();
			}
			++cycle_length;
		} while(original != dancers && cycle_length < 1'000'000'000);
		return cycle_length;
	}

	std::string part_2() override {
		const std::size_t cycle_length = find_cycle_length();
		if(cycle_length != 1'000'000'000) {
			const std::size_t extras = 1'000'000'000 % cycle_length;
			std::iota(std::begin(dancers), std::end(dancers), gsl::narrow<uint8_t>('a'));
			for(std::size_t i = 0; i < extras; ++i) {
				for(const auto& m : moves) {
					m();
				}
			}
		}
		return dancers;
	}
};
