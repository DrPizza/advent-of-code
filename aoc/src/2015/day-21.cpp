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

	static constexpr inline std::array<item, 5> weapons{ {
		{  8, 4, 0 },
		{ 10, 5, 0 },
		{ 25, 6, 0 },
		{ 40, 7, 0 },
		{ 74, 8, 0 }
	} };

	static constexpr inline std::array<item, 6> armours{ {
		{   0, 0, 0 },
		{  13, 0, 1 },
		{  31, 0, 2 },
		{  53, 0, 3 },
		{  75, 0, 4 },
		{ 102, 0, 5 }
	} };

	static constexpr inline std::array<item, 8> rings{ {
		{   0, 0, 0 },
		{   0, 0, 0 },
		{  25, 1, 0 },
		{  50, 2, 0 },
		{ 100, 3, 0 },
		{  20, 0, 1 },
		{  40, 0, 2 },
		{  80, 0, 3 }
	} };

	struct character
	{
		std::ptrdiff_t hp;
		std::ptrdiff_t damage;
		std::ptrdiff_t armour;
	};

	character boss = { 0, 0, 0 };
	character me = { 100, 0, 0 };

	void prepare_input(std::ifstream& fin) override {
		std::string boss_description;
		for(std::string line; std::getline(fin, line); ) {
			boss_description += line;
			boss_description += " ";
		}
		std::regex pattern(R"(Hit Points: ([[:digit:]]+) Damage: ([[:digit:]]+) Armor: ([[:digit:]]+).*)");
		std::smatch m;
		std::regex_search(boss_description, m, pattern);
		boss.hp = std::stoll(m[1]);
		boss.damage = std::stoll(m[2]);
		boss.armour = std::stoll(m[3]);

		me.hp = 100;
		me.damage = 0;
		me.armour = 0;
	}

	std::ptrdiff_t minimum_gold = std::numeric_limits<std::ptrdiff_t>::max();
	std::ptrdiff_t maximum_gold = std::numeric_limits<std::ptrdiff_t>::min();

	void precompute() noexcept override {
		for(const item& w : weapons) {
			for(const item& a : armours) {
				for(const item& r1 : rings) {
					for(const item& r2 : rings) {
						if(r1.cost != r2.cost || r1.cost == 0) {
							me.damage = w.damage + a.damage + r1.damage + r2.damage;
							me.armour = w.armour + a.armour + r1.armour + r2.armour;
							const std::ptrdiff_t damage_done     = std::max(1i64, me.damage - boss.armour);
							const std::ptrdiff_t damage_received = std::max(1i64, boss.damage - me.armour);

							std::ptrdiff_t my_remaining = me.hp;
							std::ptrdiff_t boss_remaining = boss.hp;
							bool i_win = false;
							for(;;) {
								boss_remaining -= damage_done;
								if(boss_remaining <= 0) {
									i_win = true;
									break;
								}
								my_remaining -= damage_received;
								if(my_remaining <= 0) {
									i_win = false;
									break;
								}
							}

							if(i_win) {
								minimum_gold = std::min(minimum_gold, w.cost + a.cost + r1.cost + r2.cost);
							} else {
								maximum_gold = std::max(maximum_gold, w.cost + a.cost + r1.cost + r2.cost);
							}
						}
					}
				}
			}
		}
	}

	std::string part_1() override {
		return std::to_string(minimum_gold);
	}

	std::string part_2() override {
		return std::to_string(maximum_gold);
	}
};

REGISTER_SOLVER(2015, 21);
