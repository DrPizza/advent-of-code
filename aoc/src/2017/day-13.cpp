#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <boost/algorithm/string.hpp>

struct advent_2017_13 : problem
{
	advent_2017_13() noexcept : problem(2017, 13) {
	}

	std::unordered_map<std::size_t, std::size_t> firewall;

	void prepare_input() override {
		std::ifstream fin("input/2017/day-13.txt");
		for(std::string line; std::getline(fin, line); ) {
			std::size_t depth = std::stoull(line.substr(0, line.find(':')));
			std::size_t range = std::stoull(line.substr(line.find(' ')));
			firewall[depth] = (range - 1) * 2;
		}
	}

	enum { caught, severity };

	auto traverse_firewall(std::size_t delay) {
		return transform_reduce(std::begin(firewall),
		                        std::end(firewall),
		                        std::make_pair(false, 0ull),
		                        [](const auto& lhs, const auto& rhs) {
		                            return std::make_pair(std::get<caught>(lhs) || std::get<caught>(rhs),
		                                                  std::get<severity>(lhs) + std::get<severity>(rhs));
		                        },
		                        [delay](const auto& layer) {
		                            const auto [depth, range] = layer;
		                            if((depth + delay) % range == 0) {
		                                return std::make_pair(true, depth * ((range / 2) + 1));
		                            } else {
		                                return std::make_pair(false, 0ull);
		                            }
		                        }
		);
	}

	std::string part_1() override {
		auto result = traverse_firewall(0);
		return std::to_string(result.second);
	}

	std::string part_2() override {
		for(std::size_t delay = 0; ; ++delay) {
			auto result = traverse_firewall(delay);
			if(!result.first) {
				return std::to_string(delay);
			}
		}
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_13>() {
	advent_2017_13 a;
	a.solve();
}
