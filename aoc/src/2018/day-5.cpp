#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <cctype>

#include <range/v3/all.hpp>

struct advent_2018_5 : problem
{
	advent_2018_5() noexcept : problem(2018, 5) {
	}

protected:
	std::string polymer;
	
	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, polymer);
	}

	std::string react(const std::string& input) {
		std::string reacted;
		for(const char ch : input) {
			if(reacted.empty()) {
				reacted.push_back(ch);
				continue;
			}
			if(std::tolower(reacted.back()) == std::tolower(ch)
			&&              reacted.back()  !=              ch ) {
				reacted.pop_back();
			} else {
				reacted.push_back(ch);
			}

		}
		return reacted;
	}

	std::string part_1() override {
		const std::string reacted = react(polymer);
		return std::to_string(reacted.size());
	}

	std::string part_2() override {
		const auto smallest = ranges::min(ranges::view::closed_iota('a', 'z')
		                                | ranges::view::transform([&] (char C) {
		                                	return react(polymer
		                                	           | ranges::view::filter([&] (char c) noexcept { return std::tolower(c) != C; })).size();
		                                }
		));
		return std::to_string(smallest);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_5>() {
	advent_2018_5 a;
	a.solve();
}
