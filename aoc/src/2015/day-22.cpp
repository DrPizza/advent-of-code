#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <array>
#include <unordered_map>

struct advent_2015_22 : problem
{
	advent_2015_22() noexcept : problem(2015, 22) {
	}

protected:
	struct character
	{
		std::int16_t hp;
		std::int16_t damage;
		std::int16_t mana;
		std::int16_t armour;
	};

	struct game_state
	{
		bool hard_mode = false;
		std::uint8_t shield_timer = 0;
		std::uint8_t poison_timer = 0;
		std::uint8_t recharge_timer = 0;
		std::uint16_t mana_spent = 0;
	};

	struct game
	{
		std::ptrdiff_t go(bool hard, character me, character boss) const noexcept {
			game_state state = { hard };
			return player_turn(me, boss, state);
		}

		std::ptrdiff_t player_turn(character me, character boss, game_state state) const noexcept {
			if(state.hard_mode) {
				--me.hp;
				if(me.hp <= 0) {
					return std::numeric_limits<std::ptrdiff_t>::max();
				}
			}

			tick_effects(me, boss, state);

			if(boss.hp <= 0) {
				return state.mana_spent;
			}

			return std::min({
				recharge(me, boss, state),
				poison  (me, boss, state),
				shield  (me, boss, state),
				drain   (me, boss, state),
				missile (me, boss, state)
			});
		}

		std::ptrdiff_t boss_turn(character me, character boss, game_state state) const noexcept {
			tick_effects(me, boss, state);
			if(boss.hp <= 0) {
				return state.mana_spent;
			}

			me.hp -= (boss.damage - me.armour);
			if(me.hp <= 0) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			return player_turn(me, boss, state);
		}

		std::ptrdiff_t recharge(character me, character boss, game_state state) const noexcept {
			static const int16_t mana_cost = 229;
			if(me.mana < mana_cost) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			if(state.recharge_timer > 0) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			me.mana -= mana_cost;
			state.mana_spent += mana_cost;

			state.recharge_timer = 5;
			return boss_turn(me, boss, state);
		}

		std::ptrdiff_t poison(character me, character boss, game_state state) const noexcept {
			static const int16_t mana_cost = 173;
			if(me.mana < mana_cost) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			if(state.poison_timer > 0) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			me.mana -= mana_cost;
			state.mana_spent += mana_cost;

			state.poison_timer = 6;
			return boss_turn(me, boss, state);
		}

		std::ptrdiff_t shield(character me, character boss, game_state state) const noexcept {
			static const int16_t mana_cost = 113;
			if(me.mana < mana_cost) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			if(state.shield_timer > 0) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			me.mana -= mana_cost;
			state.mana_spent += mana_cost;

			state.shield_timer = 6;
			return boss_turn(me, boss, state);
		}

		std::ptrdiff_t drain(character me, character boss, game_state state) const noexcept {
			static const int16_t mana_cost = 73;
			if(me.mana < mana_cost) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			me.mana -= mana_cost;
			state.mana_spent += mana_cost;

			me.hp += 2;
			boss.hp -= 2;
			if(boss.hp <= 0) {
				return state.mana_spent;
			}
			return boss_turn(me, boss, state);
		}

		std::ptrdiff_t missile(character me, character boss, game_state state) const noexcept {
			static const int16_t mana_cost = 53;
			if(me.mana < mana_cost) {
				return std::numeric_limits<std::ptrdiff_t>::max();
			}
			me.mana -= mana_cost;
			state.mana_spent += mana_cost;

			boss.hp -= 4;
			if(boss.hp <= 0) {
				return state.mana_spent;
			}
			return boss_turn(me, boss, state);
		}

		void tick_effects(character& me, character& boss, game_state& state) const noexcept {
			if(state.shield_timer > 0) {
				--state.shield_timer;
				me.armour = 7;
			} else {
				me.armour = 0;
			}
			if(state.poison_timer > 0) {
				--state.poison_timer;
				boss.hp -= 3;
			}
			if(state.recharge_timer > 0) {
				--state.recharge_timer;
				me.mana += 101;
			}
		}
	};

	character boss = {};

	void prepare_input(std::ifstream& fin) override {
		std::string boss_description;
		for(std::string line; std::getline(fin, line); ) {
			boss_description += line;
			boss_description += " ";
		}
		std::regex pattern(R"(Hit Points: ([[:digit:]]+) Damage: ([[:digit:]]+).*)");
		std::smatch m;
		std::regex_search(boss_description, m, pattern);
		boss.hp = gsl::narrow_cast<int16_t>(std::stoi(m[1]));
		boss.damage = gsl::narrow_cast<int16_t>(std::stoi(m[2]));
		boss.mana = 0;
		boss.armour = 0;
	}

	void precompute() noexcept override {
	}

	std::string part_1() override {
		game g{};
		character me = { 50, 0, 500, 0 };
		const std::ptrdiff_t minimum_mana = g.go(false, me, boss);
		return std::to_string(minimum_mana);
	}

	std::string part_2() override {
		game g{};
		character me = { 50, 0, 500, 0 };
		const std::ptrdiff_t minimum_mana = g.go(true, me, boss);
		return std::to_string(minimum_mana);
	}
};

REGISTER_SOLVER(2015, 22);
