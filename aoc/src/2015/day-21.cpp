#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <array>

struct advent_2015_21 : problem
{
	advent_2015_21() noexcept : problem(2015, 21) {
	}

protected:
	struct item
	{
		std::ptrdiff_t cost;
		std::ptrdiff_t damage;
		std::ptrdiff_t armour;
	};

	std::array<item, 5> weapons{ {
		{ 8, 4, 0 },
		{ 10, 5, 0 },
		{ 25, 6, 0 },
		{ 40, 7, 0 },
		{ 74, 8, 0 }
	} };

	std::array<item, 5> armours{ {
		{13, 0, 1},
		{31, 0, 2},
		{53, 0, 3},
		{75, 0, 4},
		{102, 0, 5}
	} };

	std::array<item, 7> rings{ {
		{0, 0, 0},
		{25, 1, 0},
		{50, 2, 0},
		{100, 3, 0},
		{20, 0, 1},
		{40, 0, 2},
		{80, 0, 3}
	} };

	struct character
	{
		std::ptrdiff_t hp;
		std::ptrdiff_t damage;
		std::ptrdiff_t armour;
	};

	character boss;

	void prepare_input(std::ifstream& fin) override {
		std::string boss_description;
		for(std::string line; std::getline(fin, line); ) {
			boss_description += line;
			boss_description += " ";
		}
		std::regex pattern(R"([^:]+: ([[:digit:]]+) [^:]+: ([[:digit:]]+) [^:]: ([[:digit:]]+).*)");
		std::smatch m;
		std::regex_search(boss_description, m, pattern);
		boss.hp = std::stoll(m[1]);
		boss.damage = std::stoll(m[2]);
		boss.armour = std::stoll(m[3]);
	}

	std::string part_1() override {
		for(const item& w : weapons) {
			for(const item& a : armours) {
				for(const item& r1 : rings) {
					for(const item& r2 : rings) {
						if(r1.cost != r2.cost || r1.cost == 0) {
							const std::ptrdiff_t damage_done     = std::max(1i64, (w.damage + a.damage + r1.damage + r2.damage) - boss.armour);
							const std::ptrdiff_t damage_received = std::max(1i64, boss.damage - (w.armour + a.armour + r1.armour + r2.damage));
							
						}
					}
				}
			}
		}
		return "";
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_21>() {
	advent_2015_21 a;
	a.solve();
}
