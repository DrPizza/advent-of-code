#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>


struct advent_2016_7 : problem
{
	advent_2016_7() noexcept : problem(2016, 7) {
	}

protected:
	std::vector<std::string> raw_addresses;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			raw_addresses.push_back(line);
		}
	}

	std::string part_1() override {
		const auto tls = [](const std::string& s) {
			bool in_hyper = false;
			bool abba = false;
			bool bad_abba = false;

			for(std::size_t i = 3; i < s.size(); ++i) {
				if(s[i - 3] == '[' || s[i - 3] == ']') {
					in_hyper = !in_hyper;
				}
				if(s[i - 3] != s[i - 2]
				&& s[i - 2] == s[i - 1]
				&& s[i - 3] == s[i - 0]) {
					abba = abba || !in_hyper;
					bad_abba = bad_abba || in_hyper;
				}
			}
			return abba && !bad_abba;
		};

		std::ptrdiff_t supports_tls = std::count_if(std::begin(raw_addresses), std::end(raw_addresses), tls);
		return std::to_string(supports_tls);
	}

	std::string part_2() override {
		const auto ssl = [](const std::string& s) {
			bool in_hyper = false;
			for(std::size_t i = 2; i < s.size(); ++i) {
				if(s[i] == '[' || s[i] == ']') {
					in_hyper = !in_hyper;
				}
				if(s[i - 2] != s[i - 1]
				&& s[i - 2] == s[i - 0]) {
					bool valid_segment = false;
					for(std::size_t j = i + 1; j < s.size() - 2; ++j) {
						if(s[j] == ']' || s[j] == '[') {
							valid_segment = !valid_segment;
						}
						if(s[j + 0] == s[i - 1]
						&& s[j + 1] == s[i - 0]
						&& s[j + 2] == s[i - 1]
						&& valid_segment) {
							return true;
						}
					}
				}
			}

			return false;
		};
		std::ptrdiff_t supports_ssl = std::count_if(std::begin(raw_addresses), std::end(raw_addresses), ssl);
		return std::to_string(supports_ssl);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_7>() {
	advent_2016_7 a;
	a.solve();
}
