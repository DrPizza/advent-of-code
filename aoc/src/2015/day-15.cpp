#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>
#include <vector>
#include <valarray>

struct advent_2015_15 : problem
{
	advent_2015_15() noexcept : problem(2015, 15) {
	}

protected:
	struct ingredient
	{
		std::string name;
		std::valarray<std::ptrdiff_t> properties;
		std::ptrdiff_t calories;
	};

	std::vector<ingredient> ingredients;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:alpha:]]+): capacity (-?[[:digit:]]+), durability (-?[[:digit:]]+), flavor (-?[[:digit:]]+), texture (-?[[:digit:]]+), calories (-?[[:digit:]]+))");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			ingredients.push_back(ingredient{ m[1].str(), {std::stoll(m[2].str()), std::stoll(m[3].str()), std::stoll(m[4].str()), std::stoll(m[5].str())}, std::stoll(m[6].str()) });
		}
	}

	template<typename F>
	void generate_counts(std::vector<std::ptrdiff_t>& amounts, std::size_t level, std::ptrdiff_t sum, F&& fn) {
		if(level == amounts.size()) {
			fn(amounts);
		} else {
			for(std::ptrdiff_t i = 0; i <= sum; ++i) {
				amounts[level] = i;
				generate_counts(amounts, level + 1, sum - i, fn);
			}
		}
	}

	std::ptrdiff_t highest_cookie = std::numeric_limits<std::ptrdiff_t>::min();
	std::ptrdiff_t highest_cookie_with_calories = std::numeric_limits<std::ptrdiff_t>::min();

	void precompute() override {
		std::vector<std::ptrdiff_t> amounts(ingredients.size());
		generate_counts(amounts, 0, 100, [&](const std::vector<std::ptrdiff_t>& amts) {
			std::valarray<std::ptrdiff_t> cookie(ingredients.size());
			std::ptrdiff_t calories = 0;
			for(std::size_t i = 0; i < amts.size(); ++i) {
				cookie += ingredients[i].properties * amts[i];
				calories += ingredients[i].calories * amts[i];
			}
			cookie = cookie.apply([](std::ptrdiff_t val) {
				return std::max(val, 0i64);
			});
			const std::ptrdiff_t cookie_score = std::accumulate(std::begin(cookie), std::end(cookie), 1i64, std::multiplies<std::ptrdiff_t>{});
			highest_cookie = std::max(highest_cookie, cookie_score);
			if(calories == 500) {
				highest_cookie_with_calories = std::max(highest_cookie_with_calories, cookie_score);
			}
		});
	}

	std::string part_1() override {
		return std::to_string(highest_cookie);
	}

	std::string part_2() override {
		return std::to_string(highest_cookie_with_calories);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_15>() {
	advent_2015_15 a;
	a.solve();
}
