#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <algorithm>
#include <numeric>

struct advent_2015_8 : problem
{
	advent_2015_8() noexcept : problem(2015, 8) {
	}

protected:
	std::vector<std::string> raw_strings;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_strings.push_back(line);
		}
	}

	std::string cook(const std::string& s) {
		bool escaped = false;
		std::string result;
		for(std::size_t i = 0; i < s.size(); ++i) {
			const char c = s[i];
			if(!escaped) {
				switch(c) {
				case '\\':
					escaped = true;
					break;
				case '"':
					break;
				default:
					result += c;
					break;
				}
			} else {
				switch(c) {
				case '\\':
					result += '\\';
					escaped = false;
					break;
				case '"':
					result += '"';
					escaped = false;
					break;
				case 'x':
					{
						const std::string hex_code = s.substr(i + 1, 2);
						result += gsl::narrow_cast<char>(std::stol(hex_code, 0, 16));
						i += 2;
						escaped = false;
					}
					break;
				}
			}
		}
		return result;
	}

	std::string uncook(const std::string& s) {
		std::string result("\"");
		for(const char c : s) {
			switch(c) {
			case '"':
				result += '\\';
				result += '"';
				break;
			case '\\':
				result += '\\';
				result += '\\';
				break;
			default:
				result += c;
				break;
			}
		}
		result += "\"";
		return result;
	}

	std::string part_1() override {
		std::vector<std::string> cooked_strings;
		for(const std::string& s : raw_strings) {
			const std::string cooked = cook(s);
			cooked_strings.push_back(cooked);
		}
		const auto add_lengths = [](std::size_t s, std::string& rhs) noexcept {
			return s + rhs.size();
		};
		const std::size_t raw_lengths    = std::accumulate(std::begin(raw_strings   ), std::end(raw_strings   ), 0ui64, add_lengths);
		const std::size_t cooked_lengths = std::accumulate(std::begin(cooked_strings), std::end(cooked_strings), 0ui64, add_lengths);
		const std::size_t difference     = raw_lengths - cooked_lengths;
		return std::to_string(difference);
	}

	std::string part_2() override {
		std::vector<std::string> uncooked_strings;
		for(const std::string& s : raw_strings) {
			const std::string uncooked = uncook(s);
			uncooked_strings.push_back(uncooked);
		}
		const auto add_lengths = [](std::size_t s, std::string& rhs) noexcept {
			return s + rhs.size();
		};
		const std::size_t uncooked_lengths = std::accumulate(std::begin(uncooked_strings), std::end(uncooked_strings), 0ui64, add_lengths);
		const std::size_t raw_lengths      = std::accumulate(std::begin(raw_strings), std::end(raw_strings), 0ui64, add_lengths);
		const std::size_t difference       = uncooked_lengths - raw_lengths;
		return std::to_string(difference);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_8>() {
	advent_2015_8 a;
	a.solve();
}
