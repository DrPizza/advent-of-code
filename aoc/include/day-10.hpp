#pragma once

#include "problem.hpp"

#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

struct advent_10 : problem
{
	advent_10() noexcept : problem(8) {
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

	void round(const std::vector<std::size_t> lengths, std::vector<std::size_t>& circle, hash_state& state) {
		for(const std::size_t length : lengths) {
			const std::size_t first = state.current_position;
			const std::size_t last = state.current_position + length - 1;
			for(std::size_t i = 0; i < (length / 2); ++i) {
				std::swap(circle[(first + i) % 256], circle[(last - i) % 256]);
			}
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

		std::vector<std::size_t> single_round;
		single_round.resize(256);
		std::iota(std::begin(single_round), std::end(single_round), 0);
		hash_state state = { 0, 0 };

		round(lengths, single_round, state);

		std::size_t first_hash = single_round[0] * single_round[1];
		return std::to_string(first_hash);
	}

	std::string part_2() override {
		std::vector<std::size_t> lengths;
		std::copy(std::begin(raw_input), std::end(raw_input), std::back_inserter(lengths));
		lengths.push_back(17);
		lengths.push_back(31);
		lengths.push_back(73);
		lengths.push_back(47);
		lengths.push_back(23);

		std::vector<std::size_t> sparse;
		sparse.resize(256);
		std::iota(std::begin(sparse), std::end(sparse), 0);
		hash_state state = { 0, 0 };
		for(std::size_t r = 0; r < 64; ++r) {
			round(lengths, sparse, state);
		}
		
		std::vector<std::size_t> dense;
		dense.resize(16);
		for(std::size_t i = 0; i < 16; ++i) {
			dense[i] = std::accumulate(std::begin(sparse) + ( i      * 16),
			                           std::begin(sparse) + ((i + 1) * 16), 0,
			                           [](std::size_t a, std::size_t b) {
				return a ^ b;
			});
		}
		std::stringstream res;
		for(std::size_t i = 0; i < 16; ++i) {
			res << std::hex << dense[i];
		}
		return res.str();
	}
};
