#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>

struct advent_2016_9 : problem
{
	advent_2016_9() noexcept : problem(2016, 9) {
	}

protected:
	std::string compressed;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, compressed);
	}

	template<typename It>
	std::size_t to_ull(It first, It last) {
		std::size_t result = 0;
		for(; first != last; ++first) {
			result *= 10;
			result += *first - '0';
		}
		return result;
	}

	template<typename It>
	std::size_t get_decoded_length(It first, It last, bool recursive) {
		static const std::regex pattern(R"(\(([[:digit:]]+)x([[:digit:]]+)\))", std::regex_constants::optimize);

		std::size_t length = 0;
		auto it = first;

		while(it != last && *it != '(') {
			++it;
			++length;
		}

		if(it == last) {
			return length;
		}

		using match = std::match_results<It>;
		match m;
		std::regex_search(it, last, m, pattern);
		const std::size_t len = to_ull(m[1].first, m[1].second);
		const std::size_t reps = to_ull(m[2].first, m[2].second);
		it += m.length(0);
		const auto suffix = it + gsl::narrow_cast<std::ptrdiff_t>(len);

		return length + (reps * (recursive ? get_decoded_length(it, suffix, true) : len)) + get_decoded_length(suffix, last, recursive);
	}

	std::string part_1() override {
		return std::to_string(get_decoded_length(std::begin(compressed), std::end(compressed), false));
	}

	std::string part_2() override {
		return std::to_string(get_decoded_length(std::begin(compressed), std::end(compressed), true));
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_9>() {
	advent_2016_9 a;
	a.solve();
}
