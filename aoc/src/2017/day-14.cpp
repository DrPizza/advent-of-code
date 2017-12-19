#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"
#include "mdspan.hpp"

#include <fstream>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <boost/algorithm/string.hpp>

struct advent_2017_14 : problem
{
	advent_2017_14() noexcept : problem(2017, 14) {
	}

	template<typename It>
	std::array<uint8_t, 256> sparse_hash(It first, It last) noexcept {
		std::array<uint8_t, 256> sparse;
		std::iota(std::begin(sparse), std::end(sparse), 0ui8);
		using cyclic = cyclic_iterator<std::array<uint8_t, 256>::iterator>;
		cyclic cfirst = cyclic(std::begin(sparse), std::end(sparse));
		std::size_t skip_size = 0;
		for(std::size_t i = 0; i < 64; ++i) {
			const auto inner_loop = [&](const uint8_t length) {
				std::reverse(cfirst, cfirst + gsl::narrow<std::ptrdiff_t>(length));
				cfirst += gsl::narrow<std::ptrdiff_t>(length + skip_size);
				++skip_size;
			};
			for(It it = first; it != last; ++it) {
				const uint8_t length = gsl::narrow<uint8_t>(*it);
				inner_loop(length);
			}
			for(const uint8_t length : { 17ui8, 31ui8, 73ui8, 47ui8, 23ui8 }) {
				inner_loop(length);
			}
		}
		return sparse;
	}

	template<typename It>
	std::array<uint8_t, 16> knot_hash(It first, It last) {
		std::array<uint8_t, 256> sparse = sparse_hash(first, last);
		std::array<uint8_t, 16> dense;
		auto out = std::begin(dense);
		for(auto it = std::begin(sparse); it != std::end(sparse); std::advance(it, 16)) {
			*out++ = std::accumulate(it, std::next(it, 16), 0ui8, std::bit_xor<uint8_t>{});
		}
		return dense;
	}

	std::string key_string;
	std::size_t set_bits = 0;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, key_string);
	}

	using grid_shape = md::extents<128, 128>;
	using grid_type  = md::mdspan<uint8_t, grid_shape>;

	std::unique_ptr<uint8_t[]> data{ std::make_unique<uint8_t[]>(128 * 128) };
	grid_type grid{ data.get() };

	void precompute() noexcept override {
		for(std::size_t i = 0; i < 128; ++i) {
			std::string row_input = key_string + "-" + std::to_string(i);
			std::array<uint8_t, 16> hash = knot_hash(std::begin(row_input), std::end(row_input));
			const gsl::span<const uint64_t> quads(reinterpret_cast<const uint64_t*>(hash.data()), 2);
			set_bits += __popcnt64(quads[0]);
			set_bits += __popcnt64(quads[1]);
			for(std::size_t j = 0; j < 16; ++j) {
				for(uint8_t k = 0; k < 8; ++k) {
					uint8_t bit = gsl::narrow<uint8_t>((hash[j] & (1ui8 << (7 - k))) >> (7 - k));
					grid(i, (j * 8) + k) = bit;
				}
			}
		}
	}

	std::string part_1() override {
		return std::to_string(set_bits);
	}

	void flood_fill(uint8_t x, uint8_t y) {
		if(grid(x, y) != 1) {
			return;
		}

		grid(x, y) = 2;
		if(x != 127) {
			flood_fill(gsl::narrow<uint8_t>(x + 1), y);
		}
		if(x != 0) {
			flood_fill(gsl::narrow<uint8_t>(x - 1), y);
		}
		if(y != 127) {
			flood_fill(x, gsl::narrow<uint8_t>(y + 1));
		}
		if(y != 0) {
			flood_fill(x, gsl::narrow<uint8_t>(y - 1));
		}
	}

	std::string part_2() override {
		std::size_t regions = 0;
		for(std::uint8_t i = 0; i < 128; ++i) {
			for(std::uint8_t j = 0; j < 128; ++j) {
				if(grid(i, j) == 1) {
					++regions;
					flood_fill(i, j);
				}
			}
		}
		return std::to_string(regions);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_14>() {
	advent_2017_14 a;
	a.solve();
}
