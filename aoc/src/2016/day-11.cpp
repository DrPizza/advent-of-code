#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <map>


enum struct floor_number : std::uint8_t
{
	bottom = 0,
	top = 3
};

enum struct direction : std::uint64_t
{
	up = 0,
	down = 1
};

enum struct item : std::uint64_t
{
	generator = 0,
	microchip = 1,
};

floor_number& operator++(floor_number& fn) noexcept {
	fn = static_cast<floor_number>(static_cast<std::uint64_t>(fn) + 1ui64);
	return fn;
}

floor_number operator+(floor_number fn, uint64_t i) noexcept {
	fn = static_cast<floor_number>(static_cast<std::uint64_t>(fn) + i);
	return fn;
}

floor_number operator-(floor_number fn, uint64_t i) noexcept {
	fn = static_cast<floor_number>(static_cast<std::uint64_t>(fn) - i);
	return fn;
}

std::map<std::string, std::pair<floor_number, floor_number>> elements;

using item_state = std::uint64_t;

constexpr item_state gm(floor_number gen, floor_number chip) {
	return 1ui64 << (static_cast<std::uint64_t>(gen) << 4ui64) << (static_cast<std::uint64_t>(chip) << 2ui64);
}

constexpr bool legal_move(item_state s) {
	return !(s & 0x88888888'88888888ui64);
}

constexpr bool compatible_items(item_state s)
{
	return !(
	        ((s & 0x00000000'0000ffffui64) && (s & 0x000f000f'000f0000ui64))
	      | ((s & 0x00000000'ffff0000ui64) && (s & 0x00f000f0'000000f0ui64))
	      | ((s & 0x0000ffff'00000000ui64) && (s & 0x0f000000'0f000f00ui64))
	      | ((s & 0xffff0000'00000000ui64) && (s & 0x0000f000'f000f000ui64))
	);
}

constexpr item_state udgmo(direction ud, item i, floor_number other) {
	return (static_cast<std::uint64_t>(ud) << 3ui64) | (static_cast<std::uint64_t>(i) << 2) | static_cast<std::uint64_t>(other);
}

item_state move_table[4][16];

void init_move_table() noexcept {
	// Default all entries to 0x8888888888888888
	std::memset(move_table, 0x88, sizeof(move_table));
	// Generate all legal moves
	for(floor_number e = floor_number::bottom; e <= floor_number::top; ++e) {
		for(floor_number o = floor_number::bottom; o <= floor_number::top; ++o) {
			if(e > floor_number::bottom) {
				move_table[static_cast<std::uint64_t>(e)][udgmo(direction::down, item::generator, o)] = gm(e - 1ui64, o) - gm(e, o);
				move_table[static_cast<std::uint64_t>(e)][udgmo(direction::down, item::microchip, o)] = gm(o, e - 1ui64) - gm(o, e);
			}
			if(e < floor_number::top) {
				move_table[static_cast<std::uint64_t>(e)][udgmo(direction::up, item::generator, o)] = gm(e + 1ui64, o) - gm(e, o);
				move_table[static_cast<std::uint64_t>(e)][udgmo(direction::up, item::microchip, o)] = gm(o, e + 1ui64) - gm(o, e);
			}
		}
	}
}

// Sign of depth (-1 if negative, 1 otherwise)
constexpr std::int8_t depth_sign(std::int8_t depth) {
	return 1 - ((depth < 0) << 1);
}

using state_t = std::pair<item_state, floor_number>;

namespace std {
	template <>
	struct hash<state_t> {
		std::size_t operator()(const state_t& s) const noexcept {
			return hash<std::uint64_t>()(std::get<0>(s)) + 33 * hash<std::uint8_t>()(static_cast<std::uint8_t>(std::get<1>(s)));
		}
	};
};

std::int8_t solve(const state_t& start, const state_t& end) {
	std::unordered_map<state_t, std::int8_t> prev, next, curr;
	curr.emplace(start, 0), curr.emplace(end, -1);
	// Breadth-first search forward and backward
	for(std::int8_t depth{ 0 }; ; ++depth) {
		for(const auto &mi0 : curr) {
			const state_t& state0 = mi0.first;
			// Select first item class to move
			for(std::uint64_t m0{ 0 }; m0 < 16ui64; ++m0) {
				// Apply move, prune if illegal
				const item_state items1 = std::get<0>(state0) + move_table[static_cast<std::uint8_t>(std::get<1>(state0))][m0];
				if(!legal_move(items1)) {
					continue;
				}
				// Select second item class to move (same up/down direction);
				// (m1 == -1 means don't move a second item)
				floor_number e(std::get<1>(state0) - ((m0 >> 2ui64) & 2ui64) + 1ui64);
				for(int m1{ -1 }; m1 < 8; ++m1) {
					uint64_t items2 = items1;
					if(m1 >= 0) {
						items2 += (m1 >= 0) * move_table[static_cast<std::uint8_t>(std::get<1>(state0))][m1 | (m0 & 8)];
					}
					// Prune if illegal move, or items not compatible
					if(!legal_move(items2) || !compatible_items(items2)) {
						continue;
					}
					decltype(prev)::const_iterator mi;
					// Check if the new state has been seen before
					if(((mi = prev.find(state_t(items2, e))) == prev.end())
					&& ((mi = curr.find(state_t(items2, e))) == curr.end())
					&& ((mi = next.find(state_t(items2, e))) == next.end())) {
						// Nope, increment depth and add to next
						next.emplace(state_t(items2, e), mi0.second + depth_sign(mi0.second));
					} else if(depth_sign(mi0.second) != depth_sign(mi->second)) {
						// Yes, and signs were opposite (solved; met in the middle)
						return gsl::narrow_cast<std::int8_t>(std::abs(mi0.second) + std::abs(mi->second));
					} // Otherwise prune
				}
			}
		}
		// Rotate the seen state memory
		prev.swap(curr), curr.swap(next), next.clear();
	}
	return -1;
}

struct advent_2016_11 : problem
{
	advent_2016_11() noexcept : problem(2016, 11) {
	}

protected:
	void prepare_input(std::ifstream& fin) override {
		static const std::regex pattern(R"(a ([[:alpha:]]+)( generator|-compatible microchip))");
		for(floor_number floor = floor_number::bottom; floor <= floor_number::top; ++floor) {
			std::string line;
			std::getline(fin, line);
			std::sregex_iterator it(std::begin(line), std::end(line), pattern), end;
			for(; it != end; ++it) {
				const auto ei = elements.find(it->str(1));
				if(ei == elements.end()) {
					elements.emplace(it->str(1), std::make_pair(floor, floor));
				} else if(it->str(2) == " generator") {
					ei->second.first = floor;
				} else {
					ei->second.second = floor;
				}
			}
		}
	}

	void precompute() noexcept override {
		init_move_table();
	}

	std::string part_1() override {
		state_t start(0, floor_number::bottom), goal(0, floor_number::top);
		for (const auto& ei : elements) {
			std::get<0>(start) += gm(ei.second.first, ei.second.second);
			std::get<0>(goal)  += gm(floor_number::top, floor_number::top);
		}
		const std::int8_t result = ::solve(start, goal);
		return std::to_string(result);
	}

	std::string part_2() override {
		state_t start(0, floor_number::bottom), goal(0, floor_number::top);
		for(const auto& ei : elements) {
			std::get<0>(start) += gm(ei.second.first, ei.second.second);
			std::get<0>(goal)  += gm(floor_number::top, floor_number::top);
		}
		std::get<0>(start) += 2 * gm(floor_number::bottom, floor_number::bottom);
		std::get<0>(goal)  += 2 * gm(floor_number::top, floor_number::top);

		const std::int8_t result = ::solve(start, goal);
		return std::to_string(result);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_11>() {
	advent_2016_11 a;
	a.solve();
}
