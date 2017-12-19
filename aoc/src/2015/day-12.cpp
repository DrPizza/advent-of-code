#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

#pragma warning(push)
#pragma warning(disable: 4061 4365 26429 26432 26434 26438 26439 26440 26462 26472 26473 26475 26481 26496 28020)
#include <nlohmann/json.hpp>
#pragma warning(pop)

using json = nlohmann::json;

struct advent_2015_12 : problem
{
	advent_2015_12() noexcept : problem(2015, 12) {
	}

protected:
	std::string input;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, input);
	}

	std::ptrdiff_t sum_values(const json& j) {
		std::ptrdiff_t total = 0;
		for(const auto& v : j) {
			if(v.is_number()) {
				total += static_cast<std::ptrdiff_t>(v);
			} else if(v.is_array() || v.is_object()) {
				total += sum_values(v);
			}
		}
		return total;
	}

	std::ptrdiff_t sum_non_red_values(const json& j) {
		std::ptrdiff_t total = 0;
		for(const auto& v : j) {
			if(v.is_number()) {
				total += static_cast<std::ptrdiff_t>(v);
			} else if(v.is_array()) {
				total += sum_non_red_values(v);
			} else if(v.is_object()) {
				bool has_red = false;
				for(json::const_iterator it = v.cbegin(); it != v.cend(); ++it) {
					if(*it == "red") {
						has_red = true;
						break;
					}
				}
				if(!has_red) {
					total += sum_non_red_values(v);
				}
			}
		}
		return total;
	}

	json data;

	void precompute() override {
		data = json::parse(input);
	}

	std::string part_1() override {
		const std::ptrdiff_t total = sum_values(data);
		return std::to_string(total);
	}

	std::string part_2() override {
		const std::ptrdiff_t total = sum_non_red_values(data);
		return std::to_string(total);
	}

};

template<>
void solve<advent_year::year_2015, advent_day::day_12>() {
	advent_2015_12 a;
	a.solve();
}
