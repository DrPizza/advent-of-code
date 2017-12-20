#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>
#include <utility>
#include <map>
#include <vector>

struct advent_2015_13 : problem
{
	advent_2015_13() noexcept : problem(2015, 13) {
	}

protected:
protected:
	std::vector<std::string> raw_happinesses;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			raw_happinesses.push_back(line);
		}
	}

	std::string part_1() override {
		using people = std::pair<std::string, std::string>;
		std::map<people, std::ptrdiff_t> happinesses;

		std::regex pattern(R"(([[:alpha:]]+) would (lose|gain) ([[:digit:]]+) happiness units by sitting next to ([[:alpha:]]+)\.)");
		for(const std::string& s : raw_happinesses) {
			std::smatch m;
			std::regex_search(s, m, pattern);
			const std::string sitter1 = m[1].str();
			const std::string sitter2 = m[4].str();
			const std::ptrdiff_t distance = (m[2].str() == "gain") ? std::stoll(m[3].str()) : -std::stoll(m[3].str());
			happinesses[std::make_pair(sitter1, sitter2)] = distance;
		}
		std::vector<std::string> person_names;
		for(const auto& pp : happinesses) {
			person_names.push_back(pp.first.first);
			person_names.push_back(pp.first.second);
		}
		std::sort(std::begin(person_names), std::end(person_names));
		person_names.erase(std::unique(std::begin(person_names), std::end(person_names)), std::end(person_names));

		std::ptrdiff_t highest_happiness = std::numeric_limits<std::ptrdiff_t>::min();
		do {
			std::ptrdiff_t current_happiness = 0;
			for(std::size_t i = 0; i < person_names.size(); ++i) {
				const std::size_t low_index = i;
				const std::size_t high_index = i + 1 == person_names.size() ? 0 : i + 1;
				current_happiness += happinesses[std::make_pair(person_names[low_index], person_names[high_index])];
				current_happiness += happinesses[std::make_pair(person_names[high_index], person_names[low_index])];
			}
			highest_happiness = std::max(highest_happiness, current_happiness);
		} while(std::next_permutation(std::begin(person_names), std::end(person_names)));
		return std::to_string(highest_happiness);
	}

	std::string part_2() override {
		using people = std::pair<std::string, std::string>;
		std::map<people, std::ptrdiff_t> happinesses;

		std::regex pattern(R"(([[:alpha:]]+) would (lose|gain) ([[:digit:]]+) happiness units by sitting next to ([[:alpha:]]+)\.)");
		for(const std::string& s : raw_happinesses) {
			std::smatch m;
			std::regex_search(s, m, pattern);
			const std::string sitter1 = m[1].str();
			const std::string sitter2 = m[4].str();
			const std::ptrdiff_t distance = (m[2].str() == "gain") ? std::stoll(m[3].str()) : -std::stoll(m[3].str());
			happinesses[std::make_pair(sitter1, sitter2)] = distance;
		}
		std::vector<std::string> person_names;
		for(const auto& pp : happinesses) {
			person_names.push_back(pp.first.first);
			person_names.push_back(pp.first.second);
		}
		std::sort(std::begin(person_names), std::end(person_names));
		person_names.erase(std::unique(std::begin(person_names), std::end(person_names)), std::end(person_names));

		for(const std::string& n : person_names) {
			happinesses[std::make_pair(n, "")] = 0;
			happinesses[std::make_pair("", n)] = 0;
		}

		person_names.push_back("");
		std::sort(std::begin(person_names), std::end(person_names));
		
		std::ptrdiff_t highest_happiness = std::numeric_limits<std::ptrdiff_t>::min();
		do {
			std::ptrdiff_t current_happiness = 0;
			for(std::size_t i = 0; i < person_names.size(); ++i) {
				const std::size_t low_index = i;
				const std::size_t high_index = i + 1 == person_names.size() ? 0 : i + 1;
				current_happiness += happinesses[std::make_pair(person_names[low_index], person_names[high_index])];
				current_happiness += happinesses[std::make_pair(person_names[high_index], person_names[low_index])];
			}
			highest_happiness = std::max(highest_happiness, current_happiness);
		} while(std::next_permutation(std::begin(person_names), std::end(person_names)));
		return std::to_string(highest_happiness);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_13>() {
	advent_2015_13 a;
	a.solve();
}
