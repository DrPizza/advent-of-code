#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <map>

struct advent_2016_6 : problem
{
	advent_2016_6() noexcept : problem(2016, 6) {
	}

protected:
	std::vector<std::string> signals;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			signals.push_back(line);
		}
	}

	std::string most_common;
	std::string least_common;

	void precompute() override {
		std::vector<std::map<char, std::size_t>> frequencies(signals[0].size());
		for(const std::string& s : signals) {
			for(std::size_t i = 0; i < s.size(); ++i) {
				++frequencies[i][s[i]];
			}
		}

		std::vector<std::multimap<std::size_t, char, std::greater<std::size_t>>> inverse_frequencies(frequencies.size());
		for(std::size_t i = 0; i < frequencies.size(); ++i) {
			for(const auto& p : frequencies[i]) {
				inverse_frequencies[i].insert(std::make_pair(p.second, p.first));
			}
		}
		for(const auto& m : inverse_frequencies) {
			most_common += m.begin()->second;
			least_common += (--m.end())->second;
		}
	}

	std::string part_1() override {
		return most_common;
	}

	std::string part_2() override {
		return least_common;
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_6>() {
	advent_2016_6 a;
	a.solve();
}
