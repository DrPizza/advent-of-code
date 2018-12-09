#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <numeric>

struct advent_2016_4 : problem
{
	advent_2016_4() noexcept : problem(2016, 4) {
	}

protected:
	struct room
	{
		std::string name;
		std::size_t sector;
		std::string checksum;
	};

	std::vector<room> rooms;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:lower:]-]+)-([[:digit:]]+)\[([[:lower:]]+)\])");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			rooms.push_back(room{ m[1].str(), std::stoull(m[2].str()), m[3].str()});
		}
	}

	std::vector<room> valid_rooms;

	void precompute() override {
		const auto validate_checksum = [](const room& r) {
			std::regex dashes(R"(-)");
			std::string name = std::regex_replace(r.name, dashes, "");

			std::map<char, std::size_t> frequencies;
			for(const char c : name) {
				++frequencies[c];
			}
			std::multimap<std::size_t, char, std::greater<std::size_t>> inverse_frequencies;
			for(const auto& p : frequencies) {
				inverse_frequencies.insert(std::make_pair(p.second, p.first));
			}
			std::size_t idx = 0;
			for(const auto& p : inverse_frequencies) {
				if(p.second != r.checksum[idx++]) {
					return false;
				}
				if(idx == 5) {
					break;
				}
			}
			return true;
		};

		std::copy_if(std::begin(rooms), std::end(rooms), std::back_inserter(valid_rooms), validate_checksum);
	}

	std::string part_1() override {
		const std::size_t sector_sum = std::accumulate(std::begin(valid_rooms), std::end(valid_rooms), 0ui64, [](std::size_t lhs, const room& r) {
			return lhs + r.sector;
		});
		return std::to_string(sector_sum);
	}

	std::string decipher(std::string input, std::size_t key) noexcept {
		for(char& c : input) {
			c = gsl::narrow_cast<char>((c == '-') ? ' ' : (((c - 'a') + gsl::narrow_cast<char>(key)) % 26) + 'a');
		}
		return input;
	}

	std::string part_2() override {
		for(const room& r : valid_rooms) {
			if(decipher(r.name, r.sector).find("north") != std::string::npos) {
				return std::to_string(r.sector);
			}
		}
		__assume(0);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_4>() {
	advent_2016_4 a;
	a.solve();
}
