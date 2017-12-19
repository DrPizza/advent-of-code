#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>
#include <utility>
#include <map>
#include <vector>

struct advent_2015_9 : problem
{
	advent_2015_9() noexcept : problem(2015, 9) {
	}

protected:
	std::vector<std::string> raw_distances;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			raw_distances.push_back(line);
		}
	}

	std::size_t lowest_distance = std::numeric_limits<std::size_t>::max();
	std::size_t highest_distance = std::numeric_limits<std::size_t>::min();

	void precompute() override {
		using cities = std::pair<std::string, std::string>;
		std::map<cities, std::size_t> distances;

		std::regex pattern(R"(([[:alpha:]]+) to ([[:alpha:]]+) = ([[:digit:]]+))");
		for(const std::string& s : raw_distances) {
			std::smatch m;
			std::regex_search(s, m, pattern);
			const std::string city1 = m[1].str();
			const std::string city2 = m[2].str();
			const std::size_t distance = std::stoull(m[3].str());
			distances[std::make_pair(std::min(city1, city2), std::max(city1, city2))] = distance;
		}
		std::vector<std::string> city_names;
		for(const auto& pp : distances) {
			city_names.push_back(pp.first.first);
			city_names.push_back(pp.first.second);
		}
		std::sort(std::begin(city_names), std::end(city_names));
		city_names.erase(std::unique(std::begin(city_names), std::end(city_names)), std::end(city_names));

		do {
			std::size_t current_distance = 0;
			for(std::size_t i = 0; i < city_names.size() - 1; ++i) {
				current_distance += distances[std::make_pair(std::min(city_names[i], city_names[i + 1]), std::max(city_names[i], city_names[i + 1]))];
			}
			lowest_distance = std::min(lowest_distance, current_distance);
			highest_distance = std::max(highest_distance, current_distance);
		} while(std::next_permutation(std::begin(city_names), std::end(city_names)));
	}

	std::string part_1() override {
		return std::to_string(lowest_distance);
	}

	std::string part_2() override {
		return std::to_string(highest_distance);
	}

};

template<>
void solve<advent_year::year_2015, advent_day::day_9>() {
	advent_2015_9 a;
	a.solve();
}
