#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <map>

#include <range/v3/all.hpp>

namespace
{
	enum struct affiliation_t
	{
		immune,
		infection
	};

	affiliation_t parse_affiliation(const std::string& s) {
		if(s == "Immune System") {
			return affiliation_t::immune;
		}
		if(s == "Infection") {
			return affiliation_t::infection;
		}
		__assume(0);
	}

	enum struct damage_t
	{
		radiation,
		fire,
		slashing,
		bludgeoning,
		cold
	};
	
	damage_t parse_damage(const std::string& s) {
		if(s == "radiation") {
			return damage_t::radiation;
		}
		if(s == "fire") {
			return damage_t::fire;
		}
		if(s == "slashing") {
			return damage_t::slashing;
		}
		if(s == "bludgeoning") {
			return damage_t::bludgeoning;
		}
		if(s == "cold") {
			return damage_t::cold;
		}
		__assume(0);
	}

	struct group_t
	{
		affiliation_t side;
		std::uintmax_t units;
		std::uintmax_t hit_points;
		std::uintmax_t attack_damage;
		damage_t damage_type;
		std::uintmax_t initiative;
		std::vector<damage_t> immunities;
		std::vector<damage_t> weaknesses;

		std::size_t effective_power() const noexcept {
			return units * attack_damage;
		}

		friend bool operator==(const group_t& lhs, const group_t& rhs) noexcept {
			return std::tie(lhs.side, lhs.units, lhs.hit_points, lhs.attack_damage, lhs.damage_type, lhs.initiative, lhs.immunities, lhs.weaknesses)
			    == std::tie(rhs.side, rhs.units, rhs.hit_points, rhs.attack_damage, rhs.damage_type, rhs.initiative, rhs.immunities, rhs.weaknesses);
		}
	};
}

struct advent_2018_24 : problem
{
	advent_2018_24() noexcept : problem(2018, 24) {
	}

protected:
	std::vector<group_t> original_groups;

	void prepare_input(std::ifstream& fin) override {
		const std::regex affiliation_pattern(R"((.*):)");
		const std::regex group_pattern(R"(([[:digit:]]+) units each with ([[:digit:]]+) hit points (\(?.*\)? )?with an attack that does ([[:digit:]]+) ([[:alnum:]]+) damage at initiative ([[:digit:]]+))");
		const std::regex modifier_pattern(R"((weak|immune) to ((?:(?:radiation|fire|slashing|bludgeoning|cold),? ?)+))");
		const std::regex modifiers_pattern(R"((radiation|fire|slashing|bludgeoning|cold))");
		affiliation_t current_affiliation = affiliation_t::immune;
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			if(std::regex_match(line, m, affiliation_pattern)) {
				current_affiliation = parse_affiliation(m[1].str());
			}
			if(std::regex_match(line, m, group_pattern)) {
				group_t g = { current_affiliation, std::stoull(m[1]), std::stoull(m[2]), std::stoull(m[4]), parse_damage(m[5].str()), std::stoull(m[6]) };

				std::string modifiers = m[3].str();
				for(std::smatch mm; std::regex_search(modifiers, mm, modifier_pattern); ) {
					std::string mods = mm[2].str();
					std::vector<std::string> raw_mods;
					auto child_it        = std::sregex_iterator(std::begin(mods), std::end(mods), modifiers_pattern);
					auto child_end       = std::sregex_iterator();
					for(; child_it != child_end; ++child_it) {
						std::smatch c = *child_it;
						if(c[1].matched) {
							raw_mods.push_back(c[1]);
						}
					}

					if(mm[1].str() == "weak") {
						g.weaknesses = ranges::view::transform(raw_mods, parse_damage);
					} else {
						g.immunities = ranges::view::transform(raw_mods, parse_damage);
					}
					modifiers = mm.suffix();
				}
				original_groups.push_back(g);
			}
		}
	}

	group_t* find_target(const group_t& warrior, std::vector<group_t>& groups, const std::vector<std::pair<group_t*, group_t*>> match_ups) const {
		auto targets = groups
		             | ranges::view::filter([&warrior, &match_ups] (group_t& candidate) {
		               	if(&candidate == &warrior
		               	|| candidate.side == warrior.side
		               	|| candidate.units == 0) {
		               		return false;
		               	}
		               	for(const auto& p : match_ups) {
		               		if(p.second == &candidate) {
		               			return false;
		               		}
		               	}
		               	if(std::find(std::begin(candidate.immunities), std::end(candidate.immunities), warrior.damage_type) != std::end(candidate.immunities)) {
		               		return false;
		               	}
		               	return true;
		               })
		             | ranges::view::transform([&warrior] (group_t& candidate) {
		               	std::uintmax_t effective_damage = warrior.effective_power();
		               	if(std::find(std::begin(candidate.weaknesses), std::end(candidate.weaknesses), warrior.damage_type) != std::end(candidate.weaknesses)) {
		               		effective_damage *= 2ui64;
		               	}
		               	return std::make_pair(effective_damage, &candidate);
		               })
		             | ranges::view::filter([](const auto& p) {
		               	return p.first != 0ui64;
		               })
		             | ranges::to_vector;
		ranges::sort(targets, [](const auto& lhs, const auto& rhs) {
			const std::uintmax_t lep = lhs.second->effective_power();
			const std::uintmax_t rep = rhs.second->effective_power();
			return std::tie(lhs.first, lep, lhs.second->initiative) > std::tie(rhs.first, rep, rhs.second->initiative);
		});
		if(targets.empty()) {
			return nullptr;
		} else {
			return targets.front().second;
		}
	}

	std::vector<group_t> run_simulation(std::vector<group_t>& groups) const {
		for(;;) {
			ranges::sort(groups, [] (const group_t& lhs, const group_t& rhs) noexcept {
				const std::uintmax_t lep = lhs.effective_power();
				const std::uintmax_t rep = rhs.effective_power();
				return std::tie(lep, lhs.initiative) > std::tie(rep, rhs.initiative);
			});

			std::vector<std::pair<group_t*, group_t*>> match_ups;
			for(group_t& g : groups) {
				if(g.units != 0) {
					group_t* target = find_target(g, groups, match_ups);
					if(target != nullptr) {
						match_ups.push_back({ &g, target });
					}
				}
			}
			if(match_ups.empty()) {
				break;
			}
			ranges::sort(match_ups, [] (const auto& lhs, const auto& rhs) {
				return lhs.first->initiative > rhs.first->initiative;
			});

			const std::vector<group_t> before = groups;
			for(auto& p : match_ups) {
				const group_t* const warrior = p.first;
				group_t* const victim = p.second;
				std::uintmax_t effective_damage = warrior->effective_power();
				if(std::find(std::begin(victim->weaknesses), std::end(victim->weaknesses), warrior->damage_type) != std::end(victim->weaknesses)) {
					effective_damage *= 2;
				}
				std::uintmax_t damage_remaining = effective_damage;
				std::uintmax_t killed = 0ui64;
				for(std::size_t i = 0; i < victim->units; ++i) {
					if(damage_remaining >= victim->hit_points) {
						damage_remaining -= victim->hit_points;
						++killed;
					}
				}
				victim->units -= killed;
			}
			const std::vector<group_t> after = groups;
			if(before == after) {
				return {};
			}
		}
		return groups;
	}

	std::string part_1() override {
		std::vector<group_t> groups = original_groups;
		groups = run_simulation(groups);
		const std::uintmax_t remaining_units = ranges::accumulate(groups, 0ui64, std::plus<void>{}, &group_t::units);
		return std::to_string(remaining_units);
	}

	std::string part_2() override {
		for(std::uintmax_t boost = 0ui64; ; ++boost) {
			std::vector<group_t> groups = original_groups;
			for(group_t& g : groups) {
				if(g.side == affiliation_t::immune) {
					g.attack_damage += boost;
				}
			}
			groups = run_simulation(groups);
			if(groups.empty()) {
				continue;
			}
			const std::uintmax_t remaining_immune = gsl::narrow_cast<std::uintmax_t>(ranges::count_if(groups, [] (const group_t& g) {
				return g.side == affiliation_t::immune && g.units > 0ui64;
			}));
			if(remaining_immune > 0ui64) {
				const std::uintmax_t remaining_units = ranges::accumulate(groups, 0ui64, std::plus<void>{}, &group_t::units);
				return std::to_string(remaining_units);
			}
		}
	}
};

REGISTER_SOLVER(2018, 24);
