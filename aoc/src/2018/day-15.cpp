#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <queue>
#include <set>
#include <vector>

namespace
{
	enum struct species
	{
		elf,
		goblin
	};

	std::string to_string(species s) {
		switch(s) {
		case species::elf:
			return "E";
		case species::goblin:
			return "G";
		}
		__assume(0);
	}

	species parse_species(const char ch) noexcept {
		if(ch == 'E') {
			return species::elf;
		} else if(ch == 'G') {
			return species::goblin;
		}
		__assume(0);
	}

	species get_enemy(species type) noexcept {
		switch(type) {
		case species::elf:
			return species::goblin;
		case species::goblin:
			return species::elf;
		}
		__assume(0);
	}

	struct point
	{
		std::intmax_t c, r;

		point& operator+=(const point& rhs) noexcept {
			c += rhs.c;
			r += rhs.r;
			return *this;
		}
		point operator+(const point& rhs) const noexcept {
			return { c + rhs.c, r + rhs.r };
		}
		bool operator==(const point& rhs) const noexcept {
			return c == rhs.c && r == rhs.r;
		}
		bool operator<(const point& rhs) const noexcept {
			return std::tie(r, c) < std::tie(rhs.r, rhs.c);
		}
	};

	std::size_t manhattan(const point& lhs, const point& rhs) noexcept {
		return std::abs(lhs.r - rhs.r) + std::abs(lhs.c - rhs.c);
	}

	struct unit
	{
		unit(std::intmax_t c, std::intmax_t r, species type_) noexcept : loc{ c, r }, type(type_) {
		}

		bool is_target_of(species t, point loc_) const noexcept {
			return hp > 0 && manhattan(loc, loc_) == 1 && t == get_enemy(type);
		}

		friend bool operator<(const unit& lhs, const unit& rhs) noexcept {
			return lhs.loc < rhs.loc;
		}

		point loc;
		species type;
		std::intmax_t hp = 200;
	};

	struct cave
	{
		cave(const std::vector<std::vector<char>>& cave, std::intmax_t elf_attack) : raw_cave(cave), elf_attack_damage(elf_attack) {
			for(std::size_t r = 0; r < raw_cave.size(); ++r) {
				for(std::size_t c = 0; c < raw_cave[r].size(); ++c) {
					if(auto s = raw_cave[r][c]; s == 'E' || s == 'G') {
						units.emplace_back(c, r, parse_species(s));
					}
				}
			}
		}

		bool do_round() {
			std::sort(std::begin(units), std::end(units));
			for(unit& me : units) {
				if(me.hp < 1) {
					continue;
				}
				if(!has_opponents(me.type)) {
					return true;
				}
				try_move(me);
				try_attack(me);
			}
			return false;
		}
		bool did_elves_die() const {
			return ranges::any_of(units, [] (auto& u) { return u.type == species::elf && u.hp < 1; });
		}
		int score() const {
			return ranges::accumulate(units, 0, [] (int i, const unit& u) { return i + (u.hp > 0 ? u.hp : 0); });
		}

	private:
		struct path
		{
			point first_step, pos;
			int length;
			bool operator<(const path& r) const noexcept {
				return std::tie(length, pos.r, pos.c, first_step.r, first_step.c)
					< std::tie(r.length, r.pos.r, r.pos.c, r.first_step.r, r.first_step.c);
			}
			bool operator==(const path& r) const noexcept {
				return std::tie(length, pos.r, pos.c, first_step.r, first_step.c)
					== std::tie(r.length, r.pos.r, r.pos.c, r.first_step.r, r.first_step.c);
			}
		};

		std::vector<path> breadth_first_search(const unit& initial) const {
			const std::vector<point> dirs{ { 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 } };

			std::queue<path> candidates;

			std::set<point> visited;
			for(const auto& d : dirs) {
				const auto pos = initial.loc + d;
				if(at(pos) == '.') {
					candidates.push(path{ d, pos, 1 });
					visited.insert({ pos.c, pos.r });
				}
			}

			std::vector<path> valid_moves;
			while(!candidates.empty()) {
				auto spot = candidates.front();
				candidates.pop();
				if(has_targets(initial.type, spot.pos)) {
					valid_moves.push_back(spot);
					continue;
				}
				for(const auto& d : dirs) {
					const point next = spot.pos + d;
					if(0 == visited.count(next) && at(next) == '.') {
						candidates.push(path{ spot.first_step, next, spot.length + 1 });
						visited.insert(next);
					}
				}
			}
			return valid_moves;
		}

		void try_move(unit& me) {
			if(has_targets(me.type, me.loc)) {
				return;
			}

			std::vector<path> valid_moves = breadth_first_search(me);
			if(valid_moves.empty()) {
				return;
			}
			std::sort(std::begin(valid_moves), std::end(valid_moves));

			at(me.loc) = '.';
			me.loc = me.loc + valid_moves.front().first_step;
			at(me.loc) = ::to_string(me.type)[0];
		}

		void try_attack(const unit& me) {
			auto targets = units
			             | ranges::view::filter([&me] (const unit& u) noexcept { return u.is_target_of(me.type, me.loc); })
			             | ranges::view::transform([] (unit& u) noexcept { return std::ref(u); })
			             | ranges::to_vector;

			if(!targets.empty()) {
				ranges::sort(targets, std::less<>(), &unit::hp);
				auto& target = targets.front().get();
				target.hp -= (me.type == species::elf ? elf_attack_damage : 3);
				if(target.hp <= 0) {
					at(target.loc) = '.';
				}
			}
		}

		bool has_targets(species type, point loc) const {
			auto targets = units
			             | ranges::view::filter([type, loc] (const unit& u) noexcept { return u.is_target_of(type, loc); });
			return !targets.empty();
		}
		bool has_opponents(species type) {
			auto targets = units
			             | ranges::view::filter([type] (const unit& u) { return u.hp > 0 && type == get_enemy(u.type); });
			return !targets.empty();
		}
		char& at(point loc) {
			return raw_cave[loc.r][loc.c];
		}

		const char& at(point loc) const {
			return raw_cave[loc.r][loc.c];
		}

		std::vector<std::vector<char>> raw_cave;
		std::vector<unit> units;
		std::intmax_t elf_attack_damage;
	};
}

struct advent_2018_15 : problem
{
	advent_2018_15() noexcept : problem(2018, 15) {
	}

protected:
	std::vector<std::vector<char>> raw_cave;

	void prepare_input(std::ifstream& fin) override {
		raw_cave = ranges::getlines(fin)
		         | ranges::view::transform([](const std::string& str) { return std::vector(std::begin(str), std::end(str)); })
		         | ranges::to_vector;
	}

	std::string part_1() override {
		std::size_t steps = 0;
		cave cavern(raw_cave, 3);
		while(!cavern.do_round()) {
			++steps;
		}

		return std::to_string(steps * cavern.score());
	}

	std::string part_2() override {
		for(std::intmax_t elf_damage = 4;; ++elf_damage) {
			cave cavern(raw_cave, elf_damage);
			std::size_t steps = 0;
			while(!cavern.do_round()) {
				++steps;
			}

			if(!cavern.did_elves_die()) {
				return std::to_string(steps * cavern.score());
			}
		}
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_15>() {
	advent_2018_15 a;
	a.solve();
}
