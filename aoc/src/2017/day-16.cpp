#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <iomanip>
#include <regex>
#include <utility>
#include <map>
#include <numeric>
#include <chrono>
#include <boost/algorithm/string.hpp>

struct advent_2017_16 : problem
{
	advent_2017_16() noexcept : problem(2017, 16) {
	}

	std::string dancers;

	void do_spin(std::ptrdiff_t amount) {
		std::rotate(std::begin(dancers), std::end(dancers) - amount, std::end(dancers));
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
		std::ifstream fin("input/2017/day-16.txt");
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

	std::map<std::size_t, std::string> generate_all_permutations() {
		std::map<std::size_t, std::string> permutations;
		std::iota(std::begin(dancers), std::end(dancers), 'a');
		const std::string original = dancers;
		do {
			permutations[permutations.size()] = dancers;
			for(const auto& m : moves) {
				m();
			}
		} while(original != dancers);
		return permutations;
	}

	std::string part_2() override {
		std::map<std::size_t, std::string> permutations = generate_all_permutations();
		const std::size_t remainder = 1'000'000'000 % permutations.size();
		return permutations[remainder];
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_16>() {
	advent_2017_16 a;
	a.solve();
}
