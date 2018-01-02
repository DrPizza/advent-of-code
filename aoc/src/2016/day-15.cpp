#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <map>
#include <regex>
#include "utility.hpp"

struct disc
{
	std::size_t size;
	std::size_t initial;
};

struct advent_2016_15 : problem
{
	advent_2016_15() noexcept : problem(2016, 15) {
	}

protected:
	std::map<std::size_t, disc> discs;

	void prepare_input(std::ifstream& fin) override {
		std::vector<std::string> lines;

		std::regex pattern(R"(Disc #([[:digit:]]+) has ([[:digit:]]+) positions; at time=0, it is at position ([[:digit:]]+).)");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			discs[std::stoull(m[1].str())] = disc{ std::stoull(m[2].str()), std::stoull(m[3].str()) };
		}
	}

	bool can_traverse_discs(std::size_t delay) {
		return utility::transform_reduce(std::begin(discs),
		                                 std::end(discs),
		                                 true,
		                                 [](const auto& lhs, const auto& rhs) {
		                                     return lhs && rhs;
		                                 },
		                                 [delay](const auto& p) {
		                                     const std::size_t position = p.first;
		                                     const disc d = p.second;
		                                     if((position + delay + d.initial) % d.size == 0) {
		                                         return true;
		                                     } else {
		                                         return false;
		                                     }
		                                 }
		);
	}

	std::string part_1() override {
		for(std::size_t delay = 0; ; ++delay) {
			const bool result = can_traverse_discs(delay);
			if(result) {
				return std::to_string(delay);
			}
		}
	}

	std::string part_2() override {
		discs[discs.size() + 1] = disc{ 11, 0 };
		for(std::size_t delay = 0; ; ++delay) {
			const bool result = can_traverse_discs(delay);
			if(result) {
				return std::to_string(delay);
			}
		}
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_15>() {
	advent_2016_15 a;
	a.solve();
}
