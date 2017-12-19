#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

struct advent_2017_12 : problem
{
	advent_2017_12() noexcept : problem(2017, 12) {
	}

	disjoint_set<std::size_t> ds;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			std::vector<std::string> fragments;
			boost::split(fragments, line, [](char c) { return c == ' '; });
			for(std::size_t i = 2; i != fragments.size(); ++i) {
				ds.merge_sets(std::stoull(fragments[0]), std::stoull(fragments[i].substr(0, fragments[i].find(','))));
			}
		}
	}

	std::string part_1() override {
		return std::to_string(ds.set_size(0u));
	}

	std::string part_2() override {
		return std::to_string(ds.count_sets());
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_12>() {
	advent_2017_12 a;
	a.solve();
}
