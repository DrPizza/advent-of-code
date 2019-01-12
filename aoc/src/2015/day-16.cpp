#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <functional>

struct advent_2015_16 : problem
{
	advent_2015_16() noexcept : problem(2015, 16) {
	}

protected:
	struct aunt
	{
		std::size_t id;
		std::unordered_map<std::string, std::size_t> properties;
	};

	std::vector<aunt> aunts;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(Sue ([[:digit:]]+): ([[:alpha:]]+): ([[:digit:]]+), ([[:alpha:]]+): ([[:digit:]]+), ([[:alpha:]]+): ([[:digit:]]+))");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			aunts.push_back(aunt{
				std::stoull(m[1].str()),
				{
					{ m[2].str(), std::stoull(m[3].str()) },
					{ m[4].str(), std::stoull(m[5].str()) },
					{ m[6].str(), std::stoull(m[7].str()) }
				}
			});
		}
	}

	using comparison_fn = std::function<bool(std::size_t, std::size_t) noexcept(false)>;

#pragma warning(push)
#pragma warning(disable: 26447)

	std::unordered_map<std::string, std::pair<comparison_fn, std::size_t>> known_properties = {
		{ "children",    { std::equal_to<std::size_t>{}, 3 } },
		{ "cats",        { std::greater <std::size_t>{}, 7 } },
		{ "samoyeds",    { std::equal_to<std::size_t>{}, 2 } },
		{ "pomeranians", { std::less    <std::size_t>{}, 3 } },
		{ "akitas",      { std::equal_to<std::size_t>{}, 0 } },
		{ "vizslas",     { std::equal_to<std::size_t>{}, 0 } },
		{ "goldfish",    { std::less    <std::size_t>{}, 5 } },
		{ "trees",       { std::greater <std::size_t>{}, 3 } },
		{ "cars",        { std::equal_to<std::size_t>{}, 2 } },
		{ "perfumes",    { std::equal_to<std::size_t>{}, 1 } }
	};

#pragma warning(pop)

	std::vector<aunt> find_plausible_aunts(bool exact_match) {
		std::vector<aunt> plausible_aunts;
		std::copy_if(std::begin(aunts), std::end(aunts), std::back_inserter(plausible_aunts), [&](const aunt& a) {
			for(const auto& prop : known_properties) {
				const auto it = a.properties.find(prop.first);
				if(it != a.properties.end()) {
					if(exact_match) {
						if(it->second != prop.second.second) {
							return false;
						}
					} else {
						if(!prop.second.first(it->second, prop.second.second)) {
							return false;
						}
					}
				}
			}
			return true;
		});
		return plausible_aunts;
	}

	std::string part_1() override {
		std::vector<aunt> plausible_aunts = find_plausible_aunts(true);
		return std::to_string(plausible_aunts[0].id);
	}

	std::string part_2() override {
		std::vector<aunt> plausible_aunts = find_plausible_aunts(false);
		return std::to_string(plausible_aunts[0].id);
	}
};

REGISTER_SOLVER(2015, 16);
