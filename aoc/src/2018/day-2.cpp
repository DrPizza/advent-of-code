#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

struct advent_2018_2 : problem
{
	advent_2018_2() noexcept : problem(2018, 2) {
	}

protected:
	std::vector<std::string> ids;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			ids.push_back(line);
		}
	}

	std::map<char, std::uintmax_t> get_counts(const std::string& s) const {
		std::map<char, std::uintmax_t> counts;
		for(const char ch : s) {
			counts[ch]++;
		}
		return counts;
	}

	std::string part_1() override {
		std::uintmax_t pairs = 0;
		std::uintmax_t triples = 0;
		for(const std::string& id : ids) {
			std::map<char, std::uintmax_t> counts = get_counts(id);
			bool has_pair = false;
			bool has_triple = false;
			for(const std::pair<char, std::uintmax_t>& p : counts) {
				if(p.second == 2) {
					has_pair = true;
				}
				if(p.second == 3) {
					has_triple = true;
				}
			}
			pairs += has_pair ? 1 : 0;
			triples += has_triple ? 1 : 0;
		}
		const std::uintmax_t checksum = pairs * triples;
		return std::to_string(checksum);
	}

	std::string part_2() override {
		std::string common_id;
		utility::for_all_pairs(std::begin(ids), std::end(ids), [&common_id] (const std::string& s1, const std::string& s2) {
			const auto first_mismatch = std::mismatch(std::begin(s1), std::end(s1), std::begin(s2));
			const auto next_mismatch  = std::mismatch(std::next(first_mismatch.first), std::end(s1), std::next(first_mismatch.second));
			if(next_mismatch.first == std::end(s1)) {
				std::copy(std::begin(s1), first_mismatch.first, std::back_inserter(common_id));
				std::copy(std::next(first_mismatch.first), std::end(s1), std::back_inserter(common_id));
			}
		});
		return common_id;
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_2>() {
	advent_2018_2 a;
	a.solve();
}
