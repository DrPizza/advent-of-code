#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>
#include <numeric>
#include <algorithm>
#include <set>

struct advent_2015_24 : problem
{
	advent_2015_24() noexcept : problem(2015, 24) {
	}

protected:
	std::vector<std::size_t> packages;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			packages.push_back(std::stoull(line));
		}
	}

	std::size_t find_best_entanglement(const std::size_t sections) {
		const std::size_t total_weight = std::accumulate(std::begin(packages), std::end(packages), 0ui64);
		const std::size_t weight_per_section = total_weight / sections;

		std::ptrdiff_t fewest_plausible_packages = 1;
		while(std::accumulate(std::rbegin(packages), std::rbegin(packages) + fewest_plausible_packages, 0ui64) < weight_per_section) {
			++fewest_plausible_packages;
		}

		for(std::ptrdiff_t r = fewest_plausible_packages; ; ++r) {
			auto cg = make_combination_generator(std::begin(packages), std::end(packages), r);
			std::vector<std::size_t> combination(gsl::narrow<std::size_t>(r), 0ui64);
			std::set<std::size_t> possible_entanglements;
			while(cg(std::begin(combination))) {
				if(weight_per_section == std::accumulate(std::begin(combination), std::end(combination), 0ui64)) {
					const std::size_t entanglement = std::accumulate(std::begin(combination), std::end(combination), 1ui64, std::multiplies<void>{});
					possible_entanglements.insert(entanglement);
				}
			}
			if(!possible_entanglements.empty()) {
				return *possible_entanglements.begin();
			}
		}
	}

	std::string part_1() override {
		return std::to_string(find_best_entanglement(3));
	}

	std::string part_2() override {
		return std::to_string(find_best_entanglement(4));
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_24>() {
	advent_2015_24 a;
	a.solve();
}
