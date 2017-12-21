#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>

struct advent_2015_19 : problem
{
	advent_2015_19() noexcept : problem(2015, 19) {
	}

protected:
	std::unordered_multimap<std::string, std::string> substitutions;
	std::string molecule;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:alpha:]]+) => ([[:alpha:]]+))");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			if(std::regex_search(line, m, pattern)) {
				substitutions.insert(std::make_pair(m[1].str(), m[2].str()));
			} else if(line.size() > 2) {
				molecule = line;
			}
		}
	}

	std::string part_1() override {
		std::unordered_set<std::string> molecules;
		for(const auto& sub : substitutions) {
			for(auto pos = molecule.find(sub.first, 0); pos != std::string::npos; pos = molecule.find(sub.first, pos + 1)) {
				std::string subbed = molecule;
				subbed.replace(pos, sub.first.length(), sub.second);
				molecules.insert(subbed);
			}
		}
		return std::to_string(molecules.size());
	}

	std::string part_2() override {
		std::string subbed = molecule;
		std::size_t steps = 0;
		while(subbed != "e") {
			for(const auto& sub : substitutions) {
				if(const auto pos = subbed.find(sub.second, 0); pos != std::string::npos) {
					++steps;
					subbed.replace(pos, sub.second.size(), sub.first);
				}
			}
		}
		return std::to_string(steps);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_19>() {
	advent_2015_19 a;
	a.solve();
}
