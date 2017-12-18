#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <array>
#include <functional>
#include <vector>
#include <type_traits>
#include <boost/algorithm/string.hpp>

struct advent_2017_10 : problem
{
	advent_2017_10() noexcept : problem(2017, 10) {
	}

	std::string raw_input;

	void prepare_input() override {
		std::ifstream fin("input/2017/day-10.txt");
		std::getline(fin, raw_input);
	}

	std::array<std::size_t, 256> partial_hash(std::size_t iterations, std::vector<std::size_t> input) noexcept {
		std::array<std::size_t, 256> sparse;
		std::iota(std::begin(sparse), std::end(sparse), 0);
		using cyclic = cyclic_iterator<std::array<std::size_t, 256>::iterator>;
		cyclic first = cyclic(std::begin(sparse), std::end(sparse));
		std::size_t skip_size = 0;
		while(iterations--) {
			for(const std::size_t length : input) {
				std::reverse(first, first + gsl::narrow<std::ptrdiff_t>(length));
				first += gsl::narrow<std::ptrdiff_t>(length + skip_size);
				++skip_size;
			}
		}
		return sparse;
	}

	std::string part_1() override {
		std::vector<std::string> words;
		boost::split(words, raw_input, [](char c) { return c == ','; });
		std::vector<std::size_t> lengths;
		std::transform(std::begin(words), std::end(words), std::back_inserter(lengths), [](const std::string& s) {
			return std::stoull(s);
		});

		std::array<std::size_t, 256> single_round = partial_hash(1, lengths);

		const std::size_t first_hash = single_round[0] * single_round[1];
		return std::to_string(first_hash);
	}

	std::string part_2() override {
		std::vector<std::size_t> lengths;
		std::copy(std::begin(raw_input), std::end(raw_input), std::back_inserter(lengths));
		lengths.insert(lengths.end(), { 17u, 31u, 73u, 47u, 23u });

		std::array<std::size_t, 256> sparse = partial_hash(64, lengths);
		
		std::vector<std::size_t> dense;
		dense.reserve(16);
		for(auto it = std::begin(sparse); it != std::end(sparse); std::advance(it, 16)) {
			dense.push_back(std::accumulate(it, std::next(it, 16), 0ull, std::bit_xor<void>{}));
		}
		std::stringstream res;
		res << std::hex << std::setfill('0');
		for(std::size_t i = 0; i < 16; ++i) {
			res << std::setw(2) << dense[i];
		}
		return res.str();
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_10>() {
	advent_2017_10 a;
	a.solve();
}
