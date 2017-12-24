#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <unordered_map>

struct advent_2015_20 : problem
{
	advent_2015_20() noexcept : problem(2015, 20) {
	}

protected:
	std::size_t target = 0;

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
		target = std::stoull(line);
	}

	std::size_t find_house(std::size_t presents_per_visit, std::size_t visit_count) {
		std::unordered_map<std::size_t, std::size_t> house_presents;
		std::size_t house_number = 0;
		for(std::size_t elf = 1; ; ++elf) {
			for(std::size_t house = elf; house <= (visit_count ? elf * visit_count : 1'000'000); house += elf) {
				house_presents[house] += elf * presents_per_visit;
			}
			if(house_presents[elf] >= target) {
				house_number = elf;
				break;
			}
		}
		return house_number;
	}

	std::string part_1() override {
		return std::to_string(find_house(10, 0));
	}

	std::string part_2() override {
		return std::to_string(find_house(11, 50));
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_20>() {
	advent_2015_20 a;
	a.solve();
}
