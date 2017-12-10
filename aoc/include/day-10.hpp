#pragma once

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <array>
#include <type_traits>
#include <boost/algorithm/string.hpp>

struct advent_10 : problem
{
	advent_10() noexcept : problem(10) {
	}

	std::string raw_input;

	void prepare_input() override {
		std::ifstream fin("day-10.txt");
		std::getline(fin, raw_input);
	}

	struct hash_state
	{
		std::size_t current_position = 0;
		std::size_t skip_size = 0;
	};

	void round(const std::vector<std::size_t> lengths, std::array<std::size_t, 256>& circle, hash_state& state) noexcept {
		for(const std::size_t length : lengths) {
			//const std::size_t first = state.current_position;
			//const std::size_t last = state.current_position + length - 1;
			//for(std::size_t i = 0; i < (length / 2); ++i) {
			//	std::swap(circle[(first + i) % 256], circle[(last - i) % 256]);
			//}
			using i_t = cyclic_iterator<std::array<std::size_t, 256>::iterator>;
			const i_t first = i_t(std::begin(circle), std::end(circle));
			std::reverse(first + gsl::narrow<std::ptrdiff_t>(state.current_position),
			             first + gsl::narrow<std::ptrdiff_t>(state.current_position + length));
			state.current_position += length;
			state.current_position += state.skip_size;
			++state.skip_size;
		}
	}

	std::string part_1() override {
		std::vector<std::string> words;
		boost::split(words, raw_input, [](char c) { return c == ','; });
		std::vector<std::size_t> lengths;
		std::transform(std::begin(words), std::end(words), std::back_inserter(lengths), [](const std::string& s) {
			return std::stoull(s);
		});

		std::array<std::size_t, 256> single_round;
		std::iota(std::begin(single_round), std::end(single_round), 0);
		hash_state state = { 0, 0 };
		round(lengths, single_round, state);

		const std::size_t first_hash = single_round[0] * single_round[1];
		return std::to_string(first_hash);
	}

	std::string part_2() override {
		std::vector<std::size_t> lengths;
		std::copy(std::begin(raw_input), std::end(raw_input), std::back_inserter(lengths));
		for(const std::size_t l : { 17u, 31u, 73u, 47u, 23u}) {
			lengths.push_back(l);
		}

		std::array<std::size_t, 256> sparse;
		std::iota(std::begin(sparse), std::end(sparse), 0);
		hash_state state = { 0, 0 };
		for(std::size_t r = 0; r < 64; ++r) {
			round(lengths, sparse, state);
		}
		
		std::vector<std::size_t> dense;
		dense.reserve(16);
		for(auto it = std::begin(sparse); it != std::end(sparse); std::advance(it, 16)) {
			dense.push_back(std::accumulate(it,
			                                std::next(it, 16),
			                                0ull,
			                                std::bit_xor<void>{}));
		}
		std::stringstream res;
		for(std::size_t i = 0; i < 16; ++i) {
			res << std::setw(2) << std::hex << dense[i];
		}
		return res.str();
	}
};
